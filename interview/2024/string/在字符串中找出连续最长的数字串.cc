/*
https://www.nowcoder.com/practice/2c81f88ecd5a4cc395b5308a99afbbec?tpId=37&tqId=21315&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D2%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void process(string &s) {
    vector<string> result;
    result.emplace_back();
    string cur;
    for (char ch : s) {
        if (isdigit(ch)) {
            cur.push_back(ch);
        } else {
            if (cur.length() >= result.back().length())
            { result.emplace_back(cur); }
            cur.clear();
        }
    }
    // NOTE!!! 循环结束要单独处理
    if (cur.length() >= result.back().length())
    { result.emplace_back(cur); }

    if (!result.empty()) {
        int maxlen = result.back().length();
        for (auto it = result.begin(); it != result.end(); ++it) {
            if (it->length() == maxlen)
            { cout << *it; }
        }
        cout << "," << maxlen << endl;
    }
}

void process1(string &s) {
    vector<string> result;

    auto s_beg = std::find_if(s.begin(), s.end(), [](char ch)->bool {
        return isdigit(ch);
    });
    while (s_beg != s.end()) {
        auto s_end = std::find_if(s_beg, s.end(), [](char ch)->bool {
            return !isdigit(ch);
        });
        result.emplace_back(s_beg, s_end);
        s_beg = std::find_if(s_end, s.end(), [](char ch)->bool {
            return isdigit(ch);
        });
    }

    if (result.empty()) {
        return;
    }

    std::sort(result.begin(), result.end(),
        [](const string& lhs, const string& rhs)->bool {
            return lhs.length() > rhs.length();
        }
    );

    int maxlen = result[0].length();
    // auto new_end = std::remove_if(result.begin(), result.end(),
        // [&](const string& s)->bool {
            // return s.length() != maxlen;
        // }
    // );
    // NOTE 若有保持原有顺序的要求用 stable_partition
    auto new_end = std::stable_partition(result.begin(), result.end(),
        [&](const string& s)->bool {
            return s.length() == maxlen;
        }
    );

    for (auto it = result.begin(); it != new_end; ++it) {
        cout << *it;
    }
    cout << "," << maxlen << endl;
}

int main() {
    string text;
    while (getline(cin, text)) {
        process(text);
    }
}
// 64 位输出请用 printf("%lld")
