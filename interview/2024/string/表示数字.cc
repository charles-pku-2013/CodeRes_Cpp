/*
https://www.nowcoder.com/practice/637062df51674de8ba464e792d1a0ac6?tpId=37&tqId=21319&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D2%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
// Jkdi234klowe90a3
// Jkdi*234*klowe*90*a*3*
#include <iostream>
using namespace std;

void process1(string &s) {
    auto s_beg = std::find_if(s.begin(), s.end(),
        [](char ch)->bool {
            return isdigit(ch);
        }
    );

    // NOTE random iterator can compare
    while (s_beg < s.end()) {
        s_beg = s.insert(s_beg, '*');  // NOTE iterator insert( iterator pos, CharT ch );
                                       // iterator insert( const_iterator pos, InputIt first, InputIt last );
                                       // basic_string& insert( size_type index, size_type count, CharT ch );
                                       // basic_string& insert( size_type index, const basic_string& str );
                                       //
                                       // NOTE replace
                                       // basic_string& replace( size_type pos, size_type count, const basic_string& str );
                                       // basic_string& replace( const_iterator first, const_iterator last, const basic_string& str );
        ++s_beg;
        auto s_end = std::find_if(s_beg, s.end(),
            [](char ch)->bool {
                return !isdigit(ch);
            }
        );
        s_end = s.insert(s_end, '*');
        ++s_end;
        s_beg = std::find_if(s_end, s.end(),
            [](char ch)->bool {
                return isdigit(ch);
            }
        );
    }
}

void process(string &s) {
    bool flag = false;
    auto it = s.begin();
    while (it != s.end()) {
        if (isdigit(*it)) {
            if (!flag) {
                flag = true;
                it = s.insert(it, '*');
                ++it;
            }
        } else { // non-digit
            if (flag) {
                flag = false;
                it = s.insert(it, '*');
                ++it;
            }
        }
        ++it;
    }
    // NOTE 末尾单独处理
    if (isdigit(s.back())) {
        s.push_back('*');
    }
}

int main() {
    string s;
    getline(cin, s);
    process(s);
    cout << s << endl;
}
// 64 位输出请用 printf("%lld")
