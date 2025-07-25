#pragma once
#include <ctranslate2/translator.h>
#include <sentencepiece_processor.h>

#include <memory>
#include <unordered_map>

namespace newtranx {
namespace ai_server {

using StringArray = std::vector<std::string>;

class Translator final {
 public:
    Translator(const std::string& s_model, const std::string& t_model);

    StringArray Translate(const StringArray& text_set /* , std::string src_language */);

    bool IsSupportedLanguage(const std::string& language) const;

 private:
    std::string s_model_;  // 分词模型
    std::string t_model_;  // 翻译模型

    std::unique_ptr<ctranslate2::Translator>               translator_;
    std::unique_ptr<sentencepiece::SentencePieceProcessor> sentencor_;

    static const std::unordered_map<std::string, std::string> supported_languages_;
};

}  // namespace ai_server
}  // namespace newtranx
