#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdint>
#include <string>


int main() {
    [](int i) {
        std::cout << i * 2 << std::endl;
    }(5);

    return 0;
}
