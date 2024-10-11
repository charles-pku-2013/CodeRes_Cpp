/*
https://www.nowcoder.com/practice/12e081cd10ee4794a2bd70c7d68f5507?tpId=37&tqId=21308&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D2%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int process(string &s) {
    auto check = [](string& str)->bool {
        return std::equal(str.begin(), str.end(), str.rbegin());  // NOTE
    };

    // NOTE 获取所有子串
    for (int len = s.length(); len >= 1; --len) {
        for (int j = 0; j <= s.length() - len; ++j) {
            string sub = s.substr(j, len);
            if (check(sub))
            { return sub.length(); }
        }
    }
    return 0;
}

int main() {
    string s;
    cin >> s;
    int len = process(s);
    cout << len << endl;
}
// 64 位输出请用 printf("%lld")
