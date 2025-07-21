/*
https://www.nowcoder.com/practice/181a1a71c7574266ad07f9739f791506?tpId=37&tqId=21288&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D1%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <string>
using namespace std;

string find_common(string &s1, string &s2) {
    if (s1.length() > s2.length())
    { s1.swap(s2); }  // s1 is shorter

    for (int len = s1.length(); len >= 1; --len) {
        for (int j = 0; j <= s1.length() - len; ++j) {
            string substr = s1.substr(j, len);
            if (s2.find(substr) != string::npos)    // NOTE find( const basic_string& str, size_type pos = 0 )
                                                    // find( CharT ch, size_type pos = 0 )
                                                    // find_first_of( const basic_string& str, size_type pos = 0 )
            { return substr; }
        }
    }

    return "";
}

int main() {
    string s, t;
    getline(cin, s);
    getline(cin, t);
    string result = find_common(s, t);
    cout << result << endl;
}
// 64 位输出请用 printf("%lld")
