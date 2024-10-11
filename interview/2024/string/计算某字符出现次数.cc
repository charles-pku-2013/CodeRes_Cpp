/*
https://www.nowcoder.com/practice/a35ce98431874e3a820dbe4b2d0508b1?tpId=37&tqId=21225&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3FtpId%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <cctype>
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
using namespace std;

int main() {
    string str;
    string line2;
    std::getline(cin, str);
    std::getline(cin, line2);
    // NOTE 字符串大小写转换这样做
    std::for_each(str.begin(), str.end(), [](char &ch){
        ch = std::tolower(ch);
    });
    std::for_each(line2.begin(), line2.end(), [](char &ch){
        ch = std::tolower(ch);
    });
    std::unordered_map<char, int> table;
    for (char ch : str)
    { table[ch]++; }

    cout << table[line2[0]] << endl;

    return 0;
}
// 64 位输出请用 printf("%lld")
