/*
https://www.nowcoder.com/practice/2aa32b378a024755a3f251e75cbf233a?tpId=37&tqId=21252&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D3%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

void encrypt(string &s) {
    std::for_each(s.begin(), s.end(),
            [](char &ch) {
        if (isalpha(ch)) {
            if (isupper(ch)) {
                ch = tolower(ch);
                if (++ch > 'z')
                { ch = 'a'; }
            } else {
                ch = toupper(ch);
                if (++ch > 'Z')
                { ch = 'A'; }
            }
        } else if (isdigit(ch)) {
            if (++ch > '9')
            { ch = '0'; }
        }
    });
}

void decrypt(string &s) {
    std::for_each(s.begin(), s.end(),
            [](char &ch) {
        if (isalpha(ch)) {
            if (isupper(ch)) {
                ch = tolower(ch);
                if (--ch < 'a')
                { ch = 'z'; }
            } else {
                ch = toupper(ch);
                if (--ch < 'A')
                { ch = 'Z'; }
            }
        } else if (isdigit(ch)) {
            if (--ch < '0')
            { ch = '9'; }
        }
    });
}

int main() {
    string text1, text2;
    getline(cin, text1);
    getline(cin, text2);

    encrypt(text1);
    decrypt(text2);

    cout << text1 << endl;
    cout << text2 << endl;
}
// 64 位输出请用 printf("%lld")
