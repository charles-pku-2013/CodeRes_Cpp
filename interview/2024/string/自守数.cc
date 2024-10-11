/*
https://www.nowcoder.com/practice/88ddd31618f04514ae3a689e83f3ab8e?tpId=37&tqId=21322&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D2%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
/*
 * 自守数是指一个数的平方的尾数等于该数自身的自然数。例如：25^2 = 625，76^2 = 5776，9376^2 = 87909376。请求出n(包括n)以内的自守数的个数
 */
#include <iostream>
#include <string>
using namespace std;

// NOTE!! int compare( size_type pos1, size_type count1, const basic_string& str )
// int compare( size_type pos1, size_type count1,
             // const basic_string& str,
             // size_type pos2, size_type count2 )
bool starts_with(const std::string& s, const std::string& prefix) {
    if (prefix.length() > s.length()) { return false; }
    return s.compare(0, prefix.length(), prefix) == 0;
}
bool ends_with(const std::string& s, const std::string& suffix) {
    if (suffix.length() > s.length()) { return false; }
    return s.compare(s.length() - suffix.length(), suffix.length(), suffix) == 0;
}

int main() {
    cout << starts_with("tfs_embedding", "tfs_") << endl;
    cout << starts_with("hello", "fuck") << endl;
    cout << ends_with("12345", "345") << endl;
    cout << ends_with("12345", "456") << endl;
    return 0;

    int n, cnt = 0;
    cin >> n;
    for (int i = 0; i <= n; ++i) {
        int square = i * i;
        string s_i = to_string(i);
        string s_square = to_string(square);
        auto pos = s_square.rfind(s_i);
        // NOTE string ends_with suffix
        if (pos != string::npos && pos + s_i.length() == s_square.length()) {
            ++cnt;
        }
    }
    cout << cnt << endl;
}
// 64 位输出请用 printf("%lld")
