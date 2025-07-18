#include "translator.h"
#include <fmt/format.h>

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

std::string Translator::Translate(const std::string& text) {


    return "";
}

}  // namespace ai_server
}  // namespace newtranx
