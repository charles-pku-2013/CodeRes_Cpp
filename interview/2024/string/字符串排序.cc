/*
https://www.nowcoder.com/practice/5190a1db6f4f4ddb92fd9c365c944584?tpId=37&tqId=21249&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D3%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <cctype>
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

string process(const string &s) {
    if (s.empty()) { return s; }

    std::unordered_multimap<char, int> alpha_index;
    std::unordered_multimap<char, int> special_index;
    for (int i = 0; i < s.length(); ++i) {
        char ch = s[i];
        if (isalpha(ch)) {
            alpha_index.insert(std::make_pair(ch, i));
        } else {
            special_index.insert(std::make_pair(ch, i));
        }
    }

    std::vector<pair<char, int>> alpha_arr(alpha_index.begin(), alpha_index.end());
    std::sort(alpha_arr.begin(), alpha_arr.end(),
            [](const pair<char, int>& lhs, const pair<char, int>& rhs)->bool {
        char left = tolower(lhs.first);
        char right = tolower(rhs.first);
        if (left != right)
        { return left < right; }
        return lhs.second < rhs.second;
    });

    std::vector<std::pair<char, int>> special_arr(special_index.begin(), special_index.end());
    std::sort(special_arr.begin(), special_arr.end(),
            [&](const pair<char, int>& lhs, const pair<char, int>& rhs)->bool {
        return lhs.second < rhs.second;
    });

    string result;
    for (auto& kv : alpha_arr)
    { result.push_back(kv.first); }

    for (auto& kv : special_arr) {
        char ch = kv.first;
        int idx = kv.second;
        if (idx >= result.length())
        { result.push_back(ch); }
        else {
            result.insert(idx, 1, ch);  // NOTE string insert(idx, count, ch) 同时支持传统iterator格式
                                        // iterator insert( iterator pos, CharT ch );
                                        // void insert( iterator pos, InputIt first, InputIt last );
        }
    }

    return result;
}

int main() {
    string text;
    getline(cin, text);
    string result = process(text);
    cout << result << endl;
}
// 64 位输出请用 printf("%lld")
