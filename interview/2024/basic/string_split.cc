#include <iostream>
#include <string>

using namespace std;

std::vector<std::string> string_split(const std::string &s, const char* delim) {
    std::vector<std::string> result;

    std::size_t beg = s.find_first_not_of(delim);
    while (beg != std::string::npos) {
        std::size_t end = s.find_first_of(delim, beg);  // NOTE 参数格式 (delim, pos)
        std::size_t count = (end == std::string::npos ? std::string::npos : end - beg); // NOTE string.substr(pos, count) 只有这一种格式
        result.emplace_back(s.substr(beg, count));
        beg = s.find_first_not_of(delim, end);          // 不使用 beg+1 end+1
    }

    return result;
}

int main() {
    std::string s("adsf-+qwret-+nvfkbdsj-+orthdfjgh-+dfjrleih");
    // std::string s("abcd");
    // std::string s;
    std::vector<std::string> arr = string_split(s, "-+");

    for (auto& s : arr) {
        cout << s << endl;
    }

    return 0;
}

