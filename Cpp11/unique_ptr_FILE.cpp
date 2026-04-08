#include <iostream>
#include <memory>
#include <cstdio>

int main() {
    // 注意 decltype fclose 必须有取地址运算符
    std::unique_ptr<FILE, decltype(&::fclose)> fp(std::fopen("test.txt", "r"), ::fclose);

    return 0;
}

