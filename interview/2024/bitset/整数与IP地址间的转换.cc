/*
https://www.nowcoder.com/practice/66ca0e28f90c42a196afd78cc9c496ea?tpId=37&tqId=21256&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D3%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
/*
 * 输入：
 * 10.0.3.193
 * 167969729
 * 输出：
 * 167773121
 * 10.3.3.193
 */
#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
using namespace std;

uint32_t ip2n(const string &s) {
    string bits;
    string seg;

    stringstream ss(s);
    while (getline(ss, seg, '.')) {
        long number = stol(seg);
        bitset<8> bin(number);
        bits.append(bin.to_string());
    }

    bitset<32> result(bits);
    return static_cast<uint32_t>(result.to_ulong());
}

string n2ip(uint32_t n) {
    string result;
    bitset<32> bits(n);
    string s = bits.to_string();
    // NOTE 固定长度切分
    for (int i = 0; i < s.length(); i += 8) {
        string seg = s.substr(i, 8);
        bitset<8> b_seg(seg);
        uint32_t n_seg = b_seg.to_ulong();
        result.append(std::to_string(n_seg)).append(".");
    }
    result.pop_back();
    return result;
}

int main() {
    string ip, num;
    getline(cin, ip);
    uint32_t n = ip2n(ip);
    cout << n << endl;

    getline(cin, num);
    n = static_cast<uint32_t>(stol(num));
    ip = n2ip(n);
    cout << ip << endl;
}
// 64 位输出请用 printf("%lld")
