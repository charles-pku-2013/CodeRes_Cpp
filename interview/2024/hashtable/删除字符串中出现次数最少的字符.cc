/*
https://www.nowcoder.com/practice/05182d328eb848dda7fdd5e029a56da9?tpId=37&tqId=21246&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D3%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
using namespace std;

string process(string s) {
    if (s.empty()) { return s; }
    std::unordered_map<char, int> cnt;
    for (char ch : s) {
        ++cnt[ch];
    }
    vector<pair<char, int>> arr(cnt.begin(), cnt.end());
    std::sort(arr.begin(), arr.end(),
            [](const pair<char, int>& lhs, const pair<char, int> &rhs)->bool {
        return lhs.second < rhs.second;
    });

    int least = arr[0].second;
    std::unordered_set<char> removed;
    for (int i = 0; i < arr.size() && arr[i].second == least; ++i) {
        char ch = arr[i].first;
        removed.insert(ch);
    }

    // NOTE
    auto it = std::remove_if(s.begin(), s.end(), [&](char ch)->bool {
        return removed.count(ch) > 0;
    });
    s.resize(std::distance(s.begin(), it));

    return s;
}

int main() {
    string text;
    getline(cin, text);
    string result = process(text);
    cout << result << endl;
}
// 64 位输出请用 printf("%lld")
