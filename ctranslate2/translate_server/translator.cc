#include "translator.h"

#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <glog/logging.h>

#include <string>

namespace newtranx {
namespace ai_server {

Translator::Translator(const std::string& s_model, const std::string& t_model) : s_model_(s_model), t_model_(t_model) {
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
    using Batch = std::vector<std::vector<std::string>>;

    std::vector<std::string> pieces;
    sentencor_->Encode(text, &pieces);
    pieces.emplace_back("</s>");  // TODO 必须加上

    DLOG(INFO) << fmt::format("pieces: {}", pieces);

    Batch batch;
    batch.emplace_back(std::move(pieces));

    DLOG(INFO) << "Translating...";
    auto translation = translator_->translate_batch(batch);

    if (translation.empty()) {
        throw std::runtime_error(fmt::format("Failed to translate '{}'.", text));
    }

    std::vector<std::string> translate_pieces;
    translate_pieces.reserve(translation[0].output().size());
    std::copy(translation[0].output().begin(), translation[0].output().end(), std::back_inserter(translate_pieces));

    DLOG(INFO) << fmt::format("translate_pieces: {}", translate_pieces);

    std::string result;
    sentencor_->Decode(translate_pieces, &result);

    return result;
}

}  // namespace ai_server
}  // namespace newtranx
