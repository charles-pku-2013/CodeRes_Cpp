#include <iostream>
#include "absl/strings/str_format.h"

// #undef __DATE__
// #undef __TIME__
// #undef __TIMESTAMP__

int main() {
    // std::cout << absl::StrFormat("%s built at %s", __FILE__, __TIMESTAMP__) << std::endl;
    // std::cout << absl::StrFormat("%s built at %s %s", __FILE__, __DATE__, __TIME__) << std::endl;
    std::cout << __date << std::endl;
    std::cout << __TIME__ << std::endl;

    return 0;
}

