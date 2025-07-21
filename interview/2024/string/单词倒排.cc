/*
https://www.nowcoder.com/practice/81544a4989df4109b33c2d65037c5836?tpId=37&tqId=38366&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D3%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <cctype>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
using namespace std;

/*
 * 1、构成单词的字符只有26个大写或小写英文字母；
 * 2、非构成单词的字符均视为单词间隔符
 */
void process(string& s) {
    if (s.empty()) {
        cout << "" << endl;
        return;
    }

    vector<string> arr;
    // auto w_beg = s.begin();
    auto w_beg = std::find_if(s.begin(), s.end(), [](char ch)->bool {
        return isalpha(ch);
    });
    // NOTE 随机迭代器可以大小比较
    // find_if 用法
    // 搜索下一个单词不要用 beg+1 end+1
    while (w_beg < s.end()) {
        auto w_end = std::find_if(w_beg, s.end(), [](char ch)->bool {
            return !isalpha(ch);
        });
        arr.emplace_back(w_beg, w_end);  // NOTE
        w_beg = std::find_if(w_end, s.end(), [](char ch)->bool {
            return isalpha(ch);
        });
    }

    std::reverse(arr.begin(), arr.end());
    for (int i = 0; i < arr.size() - 1; ++i) {
        cout << arr[i] << " ";
    }
    cout << arr.back() << endl;
}

int main() {
    string text;
    getline(cin, text);
    process(text);
}
// 64 位输出请用 printf("%lld")
