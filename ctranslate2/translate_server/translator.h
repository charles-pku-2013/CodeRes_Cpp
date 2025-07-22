#pragma once
#include <ctranslate2/translator.h>
#include <sentencepiece_processor.h>

#include <memory>

namespace newtranx {
namespace ai_server {

class Translator final {
 public:
    Translator(const std::string& s_model, const std::string& t_model);

    std::string Translate(const std::string& text);

 private:
    std::string s_model_;  // 分词模型
    std::string t_model_;  // 翻译模型

    std::unique_ptr<ctranslate2::Translator>               translator_;
    std::unique_ptr<sentencepiece::SentencePieceProcessor> sentencor_;
};

}  // namespace ai_server
}  // namespace newtranx
