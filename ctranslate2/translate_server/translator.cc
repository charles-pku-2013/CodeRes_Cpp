#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <glog/logging.h>

#include <string>

#include "translator.h"

namespace newtranx {
namespace ai_server {

Translator::Translator(const std::string& s_model, const std::string& t_model)
    : s_model_(s_model), t_model_(t_model) {
    // init sentence
    sentencor_.reset(new sentencepiece::SentencePieceProcessor);
    auto status = sentencor_->Load(s_model_);

    if (!status.ok()) {
        throw std::runtime_error(fmt::format("Load sentence model fail: {}.", status.ToString()));
    }

    const ctranslate2::models::ModelLoader model_loader(t_model_);
    translator_ = std::make_unique<ctranslate2::Translator>(model_loader);
}

StringArray Translator::Translate(const StringArray& text_set /* , std::string src_language */) {
    using Batch = std::vector<std::vector<std::string>>;

    Batch batch;

    for (const auto& text : text_set) {
        std::vector<std::string> pieces;
        sentencor_->Encode(text, &pieces);
        pieces.emplace_back("</s>");  // TODO 必须加上
                                      // TODO src_language
        batch.emplace_back(std::move(pieces));
    }

    DLOG(INFO) << fmt::format("Translating {} ...", batch);
    auto translations = translator_->translate_batch(batch);

    if (translations.empty()) {
        throw std::runtime_error(fmt::format("Failed to translate '{}'.", batch));
    }

    StringArray results;
    results.reserve(translations.size());

    for (auto& translation : translations) {
        std::vector<std::string> translate_pieces;
        translate_pieces.reserve(translation.output().size());
        std::copy(translation.output().begin(), translation.output().end(),
                  std::back_inserter(translate_pieces));
        std::string result;
        sentencor_->Decode(translate_pieces, &result);
        results.emplace_back(std::move(result));
    }

    return results;
}

bool Translator::IsSupportedLanguage(const std::string& language) const {
    return supported_languages_.count(language) != 0;
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
