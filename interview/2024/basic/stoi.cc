#include <iostream>
#include <string>

int main() {
    std::string s(" 10a ");  // 10 3 前导空格也算
    std::size_t len = 0;

    try {
        int n = std::stoi(s, &len, 10);
        std::cout << n << "\t" << len << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Conversion fail: " << ex.what() << std::endl;
    }

    return 0;
}

