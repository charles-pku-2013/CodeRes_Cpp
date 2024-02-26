#include <iostream>
#include <string>
// #include <string_view>

int main(int argc, char* argv[]) {
    // std::string s = "9d1b5f49b32940f19c56d648b09a433f";
    std::string s = "8b09a433f";
    long n = std::stol(s.substr(0, 500), nullptr, 16);
    std::cout << n << std::endl;

    return 0;
}
