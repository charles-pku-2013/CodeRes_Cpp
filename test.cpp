#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>

using namespace std;

// 123 + 456

std::string stringAddHelper(std::string& s1, std::string& s2) {
    std::reverse(s1.begin(), s1.end());
    std::reverse(s2.begin(), s2.end());

    std::size_t len = s1.length() > s2.length() ? s1.length() : s2.length();

    s1.resize(len, '0');
    s2.resize(len, '0');
    std::string result;

    auto char_add = [](char a, char b)->int {
        return (a - '0') + (b - '0');
    };

    int flag = 0;
    for (std::size_t i = 0; i < len; ++i) {
        int sum = char_add(s1[i], s2[i]) + flag;
        if (sum >= 10) {
            flag = 1;
            sum = sum % 10;
        } else {
            flag = 0;
        }
        result.push_back(sum + '0');
    }

    if (flag) {
        result.push_back('1');
    }

    while (result.back() == '0') {
        result.pop_back();
    }

    std::reverse(result.begin(), result.end());
    return result;
}

int main() {
    string s1("999");
    string s2("1");
    cout << stringAddHelper(s1, s2) << endl;

    return 0;
}
