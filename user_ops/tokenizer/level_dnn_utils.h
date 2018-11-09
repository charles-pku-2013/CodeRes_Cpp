#ifndef _LEVEL_DNN_UTILS_H
#define _LEVEL_DNN_UTILS_H

#include <string>
#include <vector>
#include <iostream>

namespace search_level {

int tokenize(const std::string &query, std::vector<std::string> *_out);

int getBigrams(const std::vector<std::string> &tokens, std::vector<std::string> *_out);

int getUnigramsAndBigrams(const std::string &query, std::vector<std::string> *_out);

}
#endif //FRAMEWORK7_LEVEL_DNN_UTILS_H
