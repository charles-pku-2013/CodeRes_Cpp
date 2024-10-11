/*
https://www.nowcoder.com/practice/4b1658fd8ffb4217bc3b7e85a38cfaf2?tpId=37&tqId=21309&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D2%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <bitset>
#include <string>
using namespace std;

int process(int n) {
    bitset<32> bits(n);
    string s = bits.to_string();
    int maxlen = 0;

    // auto b_beg = s.find_first_of("1");
    // while (b_beg != string::npos) {
        // auto b_end = s.find_first_of("0", b_beg);  // NOTE!!! end 可能是npos，不可以通过end - beg这样简单计算长度
        // string sub = s.substr(b_beg, b_end - b_beg);
        // if (sub.length() > maxlen) {
            // maxlen = sub.length();
        // }
        // b_beg = s.find_first_of("1", b_end);
    // }
    
    int len = 0;
    for (char ch : s) {
        if (ch == '1') {
            if (++len > maxlen)
            { maxlen = len; }
        } else {
            len = 0;
        }
    }

    return maxlen;
}

int main() {
    int n;
    cin >> n;
    int ret = process(n);
    cout << ret << endl;
}
// 64 位输出请用 printf("%lld")
