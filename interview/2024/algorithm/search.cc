#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    std::string s("Hello world");
    std::string t("world");

    // NOTE 和find区别，find是元素匹配，search是区间匹配
    auto it = std::search(s.begin(), s.end(), t.begin(), t.end());

    if (it != s.end()) {
        cout << *it << endl;
        cout << std::distance(s.begin(), it) << endl;
    }

    return 0;
}

