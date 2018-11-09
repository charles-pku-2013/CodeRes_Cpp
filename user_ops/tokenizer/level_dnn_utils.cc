#include "level_dnn_utils.h"
#include <cwchar>
#include <cctype>
#include <clocale>

namespace search_level {

inline
bool _set_locale() {
    std::locale::global(std::locale(""));
    return true;
}
static const bool _dummy_set_locale = _set_locale();

inline
wchar_t to_wchar(const char *src, int len, std::mbstate_t *state)
{
    wchar_t wch = L'\0';

    int iLen = std::mbrtowc(&wch, src, len, state);
    if (iLen <= 0) {
        wch = L'\0';
    }

    return wch;
}

inline
int n_octets(unsigned char lb) {
    if (( lb & 0x80 ) == 0 )          // lead bit is zero, must be a single ascii
        return 1;
    else if (( lb & 0xE0 ) == 0xC0 )  // 110x xxxx
        return 2;
    else if (( lb & 0xF0 ) == 0xE0 ) // 1110 xxxx
        return 3;
    else if (( lb & 0xF8 ) == 0xF0 ) // 1111 0xxx
        return 4;
    else
        return -1;
}

int tokenize(const std::string &query, std::vector<std::string> *_out) {
    auto &out = *_out;
    const char *pCur = query.c_str();
    const char *pEnd = query.c_str() + query.length();
    std::mbstate_t state = std::mbstate_t();

    auto is_delim = [](char ch)->bool {
        // 只保留字母和数字
        return !isalpha(ch) && !isdigit(ch);
    };

    out.reserve(query.length());
    while (pCur < pEnd) {
        int n = n_octets(*pCur);
        if (n > 1) { // chinese char
            wchar_t wchar = to_wchar(pCur, n, &state);
            if (wchar >= L'\u4e00' && wchar <= L'\u9fff') {
                out.emplace_back(pCur, n);
            }
            pCur += n;
        } else if (n == 1) { // ascii char
            // find begin of a word
            while (pCur < pEnd && n_octets(*pCur) == 1 && is_delim(*pCur)) { ++pCur; }
            const char *pLast = pCur;
            // find end of a word
            while (pCur < pEnd && n_octets(*pCur) == 1 && !is_delim(*pCur)) { ++pCur; }
            auto len = pCur - pLast;
            if (len) {
                out.emplace_back(pLast, len);
            }
        } else {    // n_octets fail
            ++pCur;
        }
    } // while

    return 0;
}

int getBigrams(const std::vector<std::string> &tokens, std::vector<std::string> *_out) {
    constexpr const char* HEAD = "^";
    constexpr const char* TAIL = "$";

    auto& out = *_out;

    if (tokens.empty()) {
        return 0;
    }

    auto sz = tokens.size();

    // set first elem
    out.emplace_back(std::string(HEAD) + " " + tokens[0]);
    // set mid elems
    int i = 1;
    for (; i < sz; ++i) {
        out.emplace_back(tokens[i-1] + " " + tokens[i]);
    }
    // set last one
    out.emplace_back(tokens[i-1] + " " + TAIL);

    return 0;
}

int getUnigramsAndBigrams(const std::string &query, std::vector<std::string> *_out) {
    auto &out = *_out;

    tokenize(query, &out);
    out.reserve(out.size() * 2 + 1);
    getBigrams(out, &out);

    return 0;
}

} // namespace _search_level
