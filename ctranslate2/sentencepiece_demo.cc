/*
See doc/api.md
c++ -o /tmp/test ctranslate2/sentencepiece_demo.cc -lsentencepiece -std=c++17 -g
/tmp/test 418M/spm.model
 */
#include <sentencepiece_processor.h>

#include <iostream>
#include <vector>

int main(int argc, char **argv) {
    sentencepiece::SentencePieceProcessor processor;
    const auto                            status = processor.Load(argv[1]);

    if (!status.ok()) {
        std::cerr << status.ToString() << std::endl;
        return -1;
    }

    {
        std::vector<std::string> pieces;
        processor.Encode("This is a test.", &pieces);
        for (const std::string &token : pieces) {
            std::cout << token << std::endl;
        }

        std::vector<int> ids;
        processor.Encode("This is a test.", &ids);
        for (const int id : ids) {
            std::cout << id << std::endl;
        }
    }

    {
        std::vector<std::string> pieces = { "▁This", "▁is", "▁a", "▁", "te", "st", "." };   // sequence of pieces
        std::string text;
        processor.Decode(pieces, &text);
        std::cout << text << std::endl;

        std::vector<int> ids = { 451, 26, 20, 3, 158, 128, 12  };   // sequence of ids
        processor.Decode(ids, &text);
        std::cout << text << std::endl;
    }

    return 0;
}
