#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string s("Hello world");
    std::string delim(" \t\f\r\v\n");
    
    auto it = std::find_first_of(s.begin(), s.end(), delim.begin(), delim.end());

    std::cout << std::string(s.begin(), it) << std::endl;

    return 0;
}

