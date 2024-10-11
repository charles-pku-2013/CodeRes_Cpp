/*
https://www.nowcoder.com/practice/995b8a548827494699dc38c3e2a54ee9?tpId=37&tqId=21313&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D2%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

bool check(string& ip) {
    stringstream ss(ip);
    string seg;
    int count = 0;
    while (getline(ss, seg, '.')) {
        ++count;
        if (seg.empty())
        { return false; }
        int n = 0;
        try {
            n = stoi(seg);
        } catch (...) {
            return false;
        }
        if (seg.length() != std::to_string(n).length())  // NOTE 检查格式严格符合，05 是非法的
        { return false; }
        if (n < 0 || n > 255)
        { return false; }
        // if (count == 1 && n == 0)
        // { return false; }
    }
    if (count != 4)
    { return false; }

    return true;
}

int main() {
    string text;
    getline(cin, text);
    bool ret = check(text);
    cout << (ret ? "YES" : "NO") << endl;
}
// 64 位输出请用 printf("%lld")
