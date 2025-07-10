/*
See docs/quickstart.md

1. Install CT2 on mac just for model convert tool
pip install ctranslate2 OpenNMT-py=='2.*' sentencepiece
check `ct2-opennmt-py-converter` exists
2. Download test model
https://s3.amazonaws.com/opennmt-models/transformer-ende-wmt-pyOnmt.tar.gz
3. Convert the model
cd transformer-ende-wmt-pyOnmt
ct2-opennmt-py-converter --model_path averaged-10-epoch.pt --output_dir ende_ctranslate2
check `model.bin` in dir ende_ctranslate2
4. Run this program (arg 1 is model dir)
/tmp/test ende_ctranslate2

c++ -o /tmp/test test.cc -lctranslate2 -std=c++17
 */
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

#include "ctranslate2/translator.h"

int main(int argc, char* argv[]) {
    const std::string                      model_path(argv[1]);  // is a directory
    const ctranslate2::models::ModelLoader model_loader(model_path);

    ctranslate2::Translator translator(model_loader);

    const std::vector<std::vector<std::string>> batch = {{"▁Hello", "▁World", "!", "</s>"}};
    const auto                                  translation = translator.translate_batch(batch);

    // for (const auto& token : translation[0].output()) {
        // std::cout << token << ' ';
    // }
    // std::cout << std::endl;

    for (const auto& trans : translation) {
        std::copy(trans.output().begin(), trans.output().end(),
                std::ostream_iterator<std::string>(std::cout , " "));
    }
    std::cout << std::endl;

    return 0;
}
