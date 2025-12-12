#include <iostream>
#include <string>
#include <type_traits>

using namespace std;

int main() {
    auto lambda = [](auto&& arg) {
        if constexpr (std::is_lvalue_reference_v<decltype(arg)>) {
            std::cout << "arg 是一个左值引用" << std::endl;
        }
        if constexpr (std::is_rvalue_reference_v<decltype(arg)>) {
            std::cout << "arg 是一个右值引用" << std::endl;
        }
    };

    std::string s = "hello";
    lambda(s);
    cout << endl;
    lambda("world");

    return 0;
}



