/*
 * Copyright, 2012-20013, JD corporation, as an unpublished work.  This file contains
 * confidential and proprietary information, including traded secrets, of JD corporation.
 * Any use, reproduction, or transmission of any part or the whole of this document is
 * expressly prohibited without the prior written permission of JD corporation
 *
 * Author: Rank team
 * Data: 2018/09/11
 */

#include "level_dnn_utils.h"
#include "boost/regex.hpp"
#include <wchar.h>
#include <string>
//using std::string;
//using std::wstring;

using namespace std;
using namespace boost;

// 解决加载找不到符号错误
namespace boost {
inline
void throw_exception(const std::exception &ex)
{ return; }
}

namespace jd_search_level {

static std::string pattern_all = "[\u4e00-\u9fa5]|[a-zA-Z0-9]+";
static std::string pattern_en= "[a-zA-Z0-9]+";
static std::string pattern_cn = "[\u4e00-\u9fa5]";

static boost::wregex wreg_all(String2Wstring(pattern_all), boost::regex::perl|boost::regbase::icase);
static boost::wregex wreg_cn(String2Wstring(pattern_cn), boost::regex::perl|boost::regbase::icase);

int64_t getSystemTime() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

std::wstring String2Wstring(string sToMatch) {
    wstring wsToMatch;
    setlocale( LC_CTYPE, "" );
    int iWLen = mbstowcs( NULL, sToMatch.c_str(), sToMatch.length() );
    if(iWLen>0) {
        wchar_t *lpwsz = new wchar_t[iWLen + 1];
        mbstowcs( lpwsz, sToMatch.c_str(), sToMatch.length() );
        wsToMatch.assign(lpwsz);
        delete []lpwsz;
    } else {
        wsToMatch=L"";
    }
    return wsToMatch;
}

std::string Wstring2String(wstring sToMatch) {
    string sResult;
    int iLen = wcstombs( NULL, sToMatch.c_str(), 0 );
    if(iLen>0) {
        char *lpsz = new char[iLen + 1];
        wcstombs( lpsz, sToMatch.c_str(), iLen );
        lpsz[iLen] = '\0';
        sResult.assign(lpsz);
        delete []lpsz;
    } else {
        sResult="";
    }
    return sResult;
}

int tokenize(const std::string &query, std::vector<std::string> *_out) {
    auto& out = *_out;

    string pattern = "[\u4e00-\u9fa5]|[a-zA-Z0-9]+";

    // std::cerr << "tokenizing run" << std::endl;
    //string str = "我是测试数据abcd09 khdskjf 数据";
    string str = query;
    wstring wstr = String2Wstring(str);
    /*
    string p1 = "[\u4e00-\u9fa5]|[a-zA-Z0-9]+";
    wstring wp1 = String2Wstring(p1);
    boost::wregex wreg(wp1, boost::regex::perl|boost::regbase::icase);
    */
    boost::smatch what;
    boost::wsmatch wswhat;
    wstring::const_iterator wstrit = wstr.begin();
    wstring::const_iterator wstrend = wstr.end();

    std::vector<std::string> raw_tokens;
    std::vector<std::string>::iterator raw_tokens_iter = raw_tokens.begin();
    while (boost::regex_search(wstrit, wstrend, wswhat, wreg_all)) {
        wstring ws1 = wswhat[0];
        string s1 = Wstring2String(ws1);
        // cout << s1 << endl;
        raw_tokens_iter = raw_tokens.insert(raw_tokens_iter, s1);
        wstrit = wswhat[0].second;
    }

    // DEBUG MESSAGE
    /*
    std::cerr << "tokenizer res : " << std::endl;
    std::cerr << "[";
    for (size_t i = 0; i < raw_tokens.size(); i++) {
        std::cerr << "raw_tokens["  << i << "]: " << raw_tokens[i] << ", ";
    }
    std::cerr << "]" << std::endl;
    */
    // 对每个原始token计算letter_tri_grams
    std::vector<std::string>::iterator out_it;
    out_it = out.begin();

    /*
    std::cout << "calculating letter tri grams" << std::endl;
    std::cerr << "raw_tokens size: " << raw_tokens.size()
              << ", " << std::endl;
    */
    for (size_t i = 0; i < raw_tokens.size(); i++) {
        std::string token;
        std::string letter_tri_gram;
        /*
        std::string pattern("[\u4e00-\u9fa5]");
        boost::wregex wreg(String2Wstring(pattern), boost::regex::perl|boost::regbase::icase);
        */
        if (!boost::regex_match(String2Wstring(raw_tokens[i]), wreg_cn)) {
            token = "#" + raw_tokens[i] + "#";
            for (size_t j = 0; j < token.size() - 2; j++) {
                letter_tri_gram = token.substr(j, j+3);
                out_it = out.insert(out_it, letter_tri_gram);
            }
        } else {
            out_it = out.insert(out_it, raw_tokens[i]);
        }
    }

    return 0;
}

int getBigrams(const std::vector<std::string> &tokens, std::vector <std::string> *_out) {
    constexpr const char* HEAD = "^";
    constexpr const char* TAIL = "$";

    auto& out = *_out;

    if (tokens.empty()) {
        return 0;
    }

    out.reserve(tokens.size() + 1);

    // set first elem
    out.emplace_back(std::string(HEAD) + " " + tokens[0]);
    // set mid elems
    for (auto it = tokens.begin()+1; it != tokens.end(); ++it) {
        out.emplace_back(*(it-1) + " " + *it);
    }
    // set last one
    out.emplace_back(tokens.back() + " " + TAIL);

    return 0;
}

int getUnigramsAndBigrams(const std::string &query, std::vector <std::string> *_out) {
    auto& out = *_out;
    std::vector<std::string> tokens;
    // std::cerr << "query: " << query << std::endl;

    int ret = tokenize(query, &tokens);

    if (ret != 0) {
        return -1;
    }

    /*
    std::cout << "unigrams: [";
    for (size_t i = 0; i < tokens.size(); i++) {
        std::cout << "t: '" << tokens[i] << "', ";
    }
    std::cout << "]" << std::endl;
    */
    out.insert(out.begin(), tokens.begin(), tokens.end());

    std::vector<std::string> bigrams;
    ret = getBigrams(tokens, &bigrams);
    if (ret != 0) {
        return -1;
    }
    /*
    std::cout << "bigrams: [";
    for (size_t i = 0; i < bigrams.size(); i++) {
        std::cout << "b: '" << bigrams[i] << "', ";
    }
    std::cout << "]" << std::endl;
    */

    out.insert(out.end(), bigrams.begin(), bigrams.end());

    return 0;
}

} // namespace jd_search_level

