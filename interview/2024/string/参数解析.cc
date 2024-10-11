/*
https://www.nowcoder.com/practice/668603dc307e4ef4bb07bcd0615ea677?tpId=37&tqId=21297&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D2%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

void parse_cmd(string &cmd) {
    string arg;
    vector<string> args; // by quotes

    args.emplace_back();
    bool quote = false;
    for (char ch : cmd) {
        switch (ch) {
            case '\"':
                quote = !quote;
                break;
            case ' ':
                if (quote) {
                    args.back().push_back(ch); // in quotes
                } else {
                    args.emplace_back();       // new
                }
                break;
            default:
                args.back().push_back(ch);
                break;
        } // switch
    }

    cout << args.size() << endl;
    for (auto& a : args) {
        cout << a << endl;
    }
}

int main() {
    string cmd;
    getline(cin, cmd);
    parse_cmd(cmd);
}
// 64 位输出请用 printf("%lld")
