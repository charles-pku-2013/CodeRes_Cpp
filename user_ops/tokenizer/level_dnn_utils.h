/*
 * Copyright, 2012-20013, JD corporation, as an unpublished work.  This file contains
 * confidential and proprietary information, including traded secrets, of JD corporation.
 * Any use, reproduction, or transmission of any part or the whole of this document is
 * expressly prohibited without the prior written permission of JD corporation
 *
 * Author: Rank team
 * Data: 2018/09/11
 */

#ifndef _LEVEL_DNN_UTILS_H
#define _LEVEL_DNN_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <sys/time.h>

namespace jd_search_level {

std::string Wstring2String(std::wstring sToMatch);

std::wstring String2Wstring(std::string sToMatch);

int tokenize(const std::string &query, std::vector<std::string> *_out);

int getBigrams(const std::vector<std::string> &tokens, std::vector<std::string> *_out);

int getUnigramsAndBigrams(const std::string &query, std::vector<std::string> *_out);

int64_t getSystemTime();

}
#endif //FRAMEWORK7_LEVEL_DNN_UTILS_H
