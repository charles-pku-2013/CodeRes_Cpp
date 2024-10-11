/*
https://www.nowcoder.com/practice/184edec193864f0985ad2684fbc86841?tpId=37&tqId=21243&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3FtpId%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <cctype>
#include <iostream>
#include <bitset>
using namespace std;

bool is_valid(string& s) {
    if (s.length() < 8)
    { return false; }

    enum {
        UPPER,
        LOWER,
        DIGIT,
        OTHER
    };
    bitset<4> flag;
    for (char ch : s) {
        if (isalpha(ch)) {
            if (ch >= 'A' && ch <= 'Z') { 
                flag.set(UPPER); 
            } else {
                flag.set(LOWER);
            }
        } else if (isdigit(ch)) {
            flag.set(DIGIT);
        } else {
            flag.set(OTHER);
        }
    } // for
    if (flag.count() < 3)
    { return false; }
    return true;
}

int main() {
    string s;
    while (cin >> s) {
        bool ret = is_valid(s);
        cout << (ret ? "OK" : "NG") << endl;
    }
}
// 64 位输出请用 printf("%lld")
