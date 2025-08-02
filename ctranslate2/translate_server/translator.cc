#include "translator.h"

#include <curl/curl.h>
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <glog/logging.h>
#include <json2pb/json_to_pb.h>
#include <json2pb/pb_to_json.h>

#include <algorithm>
#include <iterator>
#include <string>

#include "translate.pb.h"

namespace newtranx {
namespace ai_server {

/**
 * @brief  libcurl 封装，CURL对象不是线程安全的，需要进行处理
 *      you must never use a single handle from more than one thread at any given time.
 *      https://curl.se/libcurl/c/threadsafe.html
 */
class CurlHandle final {
 public:
    CurlHandle(const std::string& url, long timeout = 5) {
        curl_ = curl_easy_init();
        if (!curl_) {
            throw std::runtime_error("Failed to init CURL!");
        }

        curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());

        headers_ = curl_slist_append(headers_, "Content-Type: application/json");
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_);

        curl_easy_setopt(curl_, CURLOPT_POST, 1L);
        curl_easy_setopt(curl_, CURLOPT_TIMEOUT, timeout);
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &CurlHandle::RequestCallback);
    }

    ~CurlHandle() {
        if (curl_) {
            curl_easy_cleanup(curl_);
        }

        if (headers_) {
            curl_slist_free_all(headers_);
        }
    }

    /**
     * @brief       发送http post请求并得到返回结果
     *
     * @param req   请求文本
     * @param res   返回文本
     *
     * @return      0 on success
     *              1 this handle is busy (used by other thread)
     *              -1 other failures
     */
    int Request(const std::string& req, std::string* res) {
        std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
        if (!lock.try_lock()) {
            return 1;
        }

        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, res);
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, req.c_str());

        CURLcode res_code = curl_easy_perform(curl_);
        if (res_code != CURLE_OK) {
            return -1;
        }

        return 0;
    }

    // http post callback
    static size_t RequestCallback(void* buffer, size_t size, size_t nmemb, void* userp) {
        std::size_t  len = size * nmemb;
        std::string* s = (std::string*)userp;
        s->append((char*)buffer, len);
        return len;
    }

 private:
    CURL*       curl_ = nullptr;
    curl_slist* headers_ = nullptr;
    std::mutex  mutex_;
};

Translator::~Translator() = default;

Translator::Translator(const std::string& s_model, const std::string& t_model,
                       const std::string& split_svr)
    : s_model_(s_model), t_model_(t_model), split_svr_(split_svr) {
    // 初始化分词器
    sentencor_.reset(new sentencepiece::SentencePieceProcessor);
    auto status = sentencor_->Load(s_model_);

    if (!status.ok()) {
        throw std::runtime_error(
            fmt::format("Load sentencepiece model fail: {}.", status.ToString()));
    }

    // 初始化翻译器
    ctranslate2::models::ModelLoader model_loader(t_model_);
    if (FLAGS_device == "gpu" || FLAGS_device == "cuda") {
        model_loader.device = ctranslate2::Device::CUDA;
        model_loader.num_replicas_per_device = FLAGS_inter_threads;
        if (FLAGS_n_devices > 0) {
            for (int i = 0; i < FLAGS_n_devices; ++i) {
                model_loader.device_indices.emplace_back(i);
            }
        }
    }
    translator_ = std::make_unique<ctranslate2::Translator>(model_loader);

    // 初始化断句服务请求队列
    curl_handle_que_.reserve(CURL_HANDLE_QUE_SZ);
    for (auto i = 0; i < CURL_HANDLE_QUE_SZ; ++i) {
        curl_handle_que_.emplace_back(new CurlHandle(split_svr_));
    }
}

std::string Translator::Translate(const std::string& article, const std::string& src_language,
                                  const std::string& dst_language) {
    if (article.empty()) {
        throw std::runtime_error("Input src article cannot be empty!");
    }

    StringArray sentences;

    // 请求断句服务器进行断句
    if (!_SplitSentence(article, src_language, &sentences)) {
        throw std::runtime_error(fmt::format("Failed to split sentence for '{}'", article));
    }

    // 删除断句后的空字句子
    sentences.erase(std::remove_if(sentences.begin(), sentences.end(),
                                   [](const auto& str) -> bool { return str.empty(); }),
                    sentences.end());

    if (sentences.empty()) {
        throw std::runtime_error(
            fmt::format("Failed to translate '{}' for 0 splitted sentences", article));
    }

    // 得到翻译后的句子集合
    auto translated_sentences = Translate(sentences, src_language, dst_language);

    // 将翻译后句子集合合并成article并返回
    return fmt::to_string(fmt::join(translated_sentences, " "));
}

StringArray Translator::Translate(const StringArray& sentences, const std::string& src_language,
                                  const std::string& dst_language) {
    using Batch = std::vector<std::vector<std::string>>;

    DLOG(INFO) << fmt::format("Translating sentences {} ...", sentences);

    Batch batch;
    for (const auto& sentence : sentences) {
        std::vector<std::string> pieces, _pieces;

        // 对每个句子进行分词
        sentencor_->Encode(sentence, &_pieces);

        // 分词结果添加前后缀
        pieces.reserve(_pieces.size() + 2);
        pieces.emplace_back(supported_languages_.at(src_language));
        std::move(_pieces.begin(), _pieces.end(), std::back_inserter(pieces));
        pieces.emplace_back("</s>");

        batch.emplace_back(std::move(pieces));
    }

    auto&                                 dst_language_tag = supported_languages_.at(dst_language);
    std::vector<std::vector<std::string>> target_prefix(batch.size(), {dst_language_tag});

    DLOG(INFO) << fmt::format("Translating batch {} ...", batch);
    auto translations = translator_->translate_batch(batch, target_prefix);

    if (translations.empty()) {
        throw std::runtime_error(
            fmt::format("Failed to translate '{}' for empty results!", sentences));
    }

    StringArray results;
    results.reserve(translations.size());

    for (auto& translation : translations) {
        // 翻译结果以分词形式存在
        std::vector<std::string> translate_pieces;
        translate_pieces.reserve(translation.output().size());
        auto beg = translation.output().begin();
        if (beg != translation.output().end() && *beg == dst_language_tag) {
            ++beg;
        }
        std::copy(std::make_move_iterator(beg), std::make_move_iterator(translation.output().end()),
                  std::back_inserter(translate_pieces));
        // 将翻译结果分词合并成句子
        std::string result;
        sentencor_->Decode(translate_pieces, &result);
        results.emplace_back(std::move(result));
    }

    DLOG(INFO) << fmt::format("Translated results {} ...", results);

    return results;
}

bool Translator::IsSupportedLanguage(const std::string& language) const {
    return supported_languages_.count(language) != 0;
}

bool Translator::_SplitSentence(const std::string& article, const std::string& src_language,
                                StringArray* result) {
    SentenceRequest split_req;
    split_req.set_lang(src_language);
    split_req.set_text(article);

    std::string req_str, err;
    if (!json2pb::ProtoMessageToJson(split_req, &req_str, json2pb::Pb2JsonOptions(), &err)) {
        LOG(ERROR) << fmt::format("SplitSentence failed to convert proto to json: '{}'", err);
        return false;
    }

    std::string res_str;
    int         status = 0;

    // 从curl handle队列中找一个空闲的handle用来处理请求，遍历5遍，若找不到就返回失败
    for (int i = 0; i < CURL_HANDLE_QUE_SZ * 5; ++i) {
        auto* handle = curl_handle_que_[++curl_handle_que_idx_ % CURL_HANDLE_QUE_SZ].get();
        status = handle->Request(req_str, &res_str);
        if (status == 0) {
            break;
        } else if (status == 1) {
            continue;  // 当前handle正忙，继续找
        } else {
            return false;  // 断句出现错误
        }
    }  // for i

    // 找不到空闲handle
    if (status == 1) {
        LOG(ERROR) << "SplitSentence no server available";
        return false;
    }

    SentenceResponse split_res;
    if (!json2pb::JsonToProtoMessage(res_str, &split_res, json2pb::Json2PbOptions{}, &err)) {
        LOG(ERROR) << fmt::format("SplitSentence failed to convert json to proto: '{}'", err);
        return false;
    }

    result->reserve(split_res.sentences_size());
    for (auto i = 0; i < split_res.sentences_size(); ++i) {
        result->emplace_back(std::move(split_res.sentences(i)));
    }

    return true;
}

std::string Translator::DebugString() const {
    return fmt::format(
        "{{"
        "ctranslate2_model: {}, sentencepiece_model: {}, "
        "sentence_split_server: {}, "
        "num_curl_handle: {}, "
        "device: {}, n_devices: {}, inter_threads: {}, "
        "}}",
        t_model_, s_model_, split_svr_, CURL_HANDLE_QUE_SZ, FLAGS_device,
        FLAGS_n_devices, FLAGS_inter_threads);
}

const std::unordered_map<std::string, std::string> Translator::supported_languages_{
    {"af", "__af__"}, {"am", "__am__"}, {"ar", "__ar__"},   {"ast", "__ast__"}, {"az", "__az__"},
    {"ba", "__ba__"}, {"be", "__be__"}, {"bg", "__bg__"},   {"bn", "__bn__"},   {"br", "__br__"},
    {"bs", "__bs__"}, {"ca", "__ca__"}, {"ceb", "__ceb__"}, {"cs", "__cs__"},   {"cy", "__cy__"},
    {"da", "__da__"}, {"de", "__de__"}, {"el", "__el__"},   {"en", "__en__"},   {"es", "__es__"},
    {"et", "__et__"}, {"fa", "__fa__"}, {"ff", "__ff__"},   {"fi", "__fi__"},   {"fr", "__fr__"},
    {"fy", "__fy__"}, {"ga", "__ga__"}, {"gd", "__gd__"},   {"gl", "__gl__"},   {"gu", "__gu__"},
    {"ha", "__ha__"}, {"he", "__he__"}, {"hi", "__hi__"},   {"hr", "__hr__"},   {"ht", "__ht__"},
    {"hu", "__hu__"}, {"hy", "__hy__"}, {"id", "__id__"},   {"ig", "__ig__"},   {"ilo", "__ilo__"},
    {"is", "__is__"}, {"it", "__it__"}, {"ja", "__ja__"},   {"jv", "__jv__"},   {"ka", "__ka__"},
    {"kk", "__kk__"}, {"km", "__km__"}, {"kn", "__kn__"},   {"ko", "__ko__"},   {"lb", "__lb__"},
    {"lg", "__lg__"}, {"ln", "__ln__"}, {"lo", "__lo__"},   {"lt", "__lt__"},   {"lv", "__lv__"},
    {"mg", "__mg__"}, {"mk", "__mk__"}, {"ml", "__ml__"},   {"mn", "__mn__"},   {"mr", "__mr__"},
    {"ms", "__ms__"}, {"my", "__my__"}, {"ne", "__ne__"},   {"nl", "__nl__"},   {"no", "__no__"},
    {"ns", "__ns__"}, {"oc", "__oc__"}, {"or", "__or__"},   {"pa", "__pa__"},   {"pl", "__pl__"},
    {"ps", "__ps__"}, {"pt", "__pt__"}, {"ro", "__ro__"},   {"ru", "__ru__"},   {"sd", "__sd__"},
    {"si", "__si__"}, {"sk", "__sk__"}, {"sl", "__sl__"},   {"so", "__so__"},   {"sq", "__sq__"},
    {"sr", "__sr__"}, {"ss", "__ss__"}, {"su", "__su__"},   {"sv", "__sv__"},   {"sw", "__sw__"},
    {"ta", "__ta__"}, {"th", "__th__"}, {"tl", "__tl__"},   {"tn", "__tn__"},   {"tr", "__tr__"},
    {"uk", "__uk__"}, {"ur", "__ur__"}, {"uz", "__uz__"},   {"vi", "__vi__"},   {"wo", "__wo__"},
    {"xh", "__xh__"}, {"yi", "__yi__"}, {"yo", "__yo__"},   {"zh", "__zh__"},   {"zu", "__zu__"}};

}  // namespace ai_server
}  // namespace newtranx
