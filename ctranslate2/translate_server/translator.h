#pragma once
#include <ctranslate2/translator.h>
#include <sentencepiece_processor.h>

#include <memory>
#include <unordered_map>
#include <vector>

namespace newtranx {
namespace ai_server {

using StringArray = std::vector<std::string>;

class CurlHandle;

class Translator final {
 public:
    Translator(const std::string& s_model, const std::string& t_model,
               const std::string& split_svr);

    ~Translator();

    /**
     * @brief
     *
     * @param article       要翻译的文章
     * @param src_language  源语种
     * @param dst_language  目标语种
     *
     * @return   翻译后的文章
     */
    std::string Translate(const std::string& article, const std::string& src_language,
                          const std::string& dst_language);

    /**
     * @brief
     *
     * @param sentences      一篇article经过断句后的句子集合
     * @param src_language   源语种
     * @param dst_language   目标语种
     *
     * @return   翻译后句子的集合
     */
    StringArray Translate(const StringArray& sentences, const std::string& src_language,
                          const std::string& dst_language);

    // 是否支持该语种翻译
    bool IsSupportedLanguage(const std::string& language) const;

 public:
    /**
     * @brief                  文章断句
     *
     * @param article          输入文章
     * @param src_language     文章语种
     * @param result           断句结果
     *
     * @return                 true on success, false on failure
     */
    bool _SplitSentence(const std::string& article, const std::string& src_language,
                        StringArray* result);

 private:
    std::string s_model_;  // 分词模型
    std::string t_model_;  // 翻译模型

    std::unique_ptr<ctranslate2::Translator>               translator_;  // 翻译器
    std::unique_ptr<sentencepiece::SentencePieceProcessor> sentencor_;   // 分词器

    // curl handle queue for sentence split
    std::string                              split_svr_;        // 断句服务器url
    std::vector<std::unique_ptr<CurlHandle>> curl_handle_que_;  // curl队列 for thread safety
    std::atomic_size_t                       curl_handle_que_idx_{0};
    static constexpr std::size_t             CURL_HANDLE_QUE_SZ = 3000;  // curl队列大小

    static const std::unordered_map<std::string, std::string>
        supported_languages_;  // 目前所支持的语种
};

}  // namespace ai_server
}  // namespace newtranx
