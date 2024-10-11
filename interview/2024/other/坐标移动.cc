/*
https://www.nowcoder.com/practice/119bcca3befb405fbe58abe9c532eb29?tpId=37&tqId=21240&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D3%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

void process(string& cmd, int &x, int &y) {
    if (cmd.empty()) { return; }
    char direction = cmd[0];
    int offset = 0;
    size_t len = 0;
    cmd = cmd.substr(1);  // remove first
                          // NOTE substr( size_type pos = 0, size_type count = npos ) 
    try {
        offset = std::stoi(cmd, &len);  // NOTE
    } catch (...) {
        return;
    }
    if (len != cmd.length())
    { return; }

    switch (direction) {
        case 'A':
            x -= offset;
            break;
        case 'S':
            y -= offset;
            break;
        case 'D':
            x += offset;
            break;
        case 'W':
            y += offset;
            break;
        default:
            return;
    } // switch
}

int main() {
    string line, cmd;
    int x = 0, y = 0;
    getline(cin, line);
    stringstream ss(line);

    while (getline(ss, cmd, ';')) {
        process(cmd, x, y);
    }

    cout << x << "," << y << endl;
}
// 64 位输出请用 printf("%lld")
