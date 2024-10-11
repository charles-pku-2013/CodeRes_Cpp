/*
https://www.nowcoder.com/practice/ca6ac6ef9538419abf6f883f7d6f6ee5?tpId=37&tqId=21289&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D1%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <string>
#include <sstream>
#include <map>
using namespace std;

map<string, string> cmd_table1;
map<string, map<string, string>> cmd_table2;

string match_cmd1(const string& cmd,
        map<string, string>& cmd_table1) {
    auto it = cmd_table1.lower_bound(cmd);
    if (it == cmd_table1.end())
    { return "unknown command"; }
    // NOTE!!! string::compare(pos, count, other) 是本字符串部分和整个other进行比较,
    // 必要时指定other的范围 string::compare(pos, count, other, pos1, count1)
    // 短的作为参数
    // 与find参数区别
    if (it->first.compare(0, cmd.length(), cmd) != 0) // no match
    { return "unknown command"; }
    auto it_bak = it;
    ++it;
    // NOTE!!! ALWAYS CHECK RANGE!!!!
    if (it != cmd_table1.end() && it->first.compare(0, cmd.length(), cmd) == 0) // more than 1
    { return "unknown command"; }
    return it_bak->second;
}

string match_cmd2(const string& cmd1, const string& cmd2,
        map<string, map<string, string>> cmd_table2) {
    auto it = cmd_table2.lower_bound(cmd1);
    if (it == cmd_table2.end())
    { return "unknown command"; }
    if (it->first.compare(0, cmd1.length(), cmd1) != 0) // no match
    { return "unknown command"; }
    auto it_bak = it;
    ++it;
    if (it != cmd_table2.end() && it->first.compare(0, cmd1.length(), cmd1) == 0) // more than 1
    { return "unknown command"; }
    return match_cmd1(cmd2, it_bak->second);
}

int main() {
    cmd_table1["reset"] = "reset what";
    cmd_table2["reset"]["board"] = "board fault";
    cmd_table2["board"]["add"] = "where to add";
    cmd_table2["board"]["delete"] = "no board at all";
    cmd_table2["reboot"]["backplane"] = "impossible";
    cmd_table2["backplane"]["abort"] = "install first";

    string line;
    while (getline(cin, line)) {
        // cout << line << endl;
        stringstream ss(line);
        string cmd1, cmd2, result;
        ss >> cmd1 >> cmd2;
        if (cmd2.empty()) {
            // one word cmd
            result = match_cmd1(cmd1, cmd_table1);
        } else {
            // 2 word cmd
            result = match_cmd2(cmd1, cmd2, cmd_table2);
        }
        cout << result << endl;
    }
}
// 64 位输出请用 printf("%lld")
