#include <iostream>
#include <string>

using namespace std;

int main() {
    std::string s1("hello");
    std::string s2("world");
    const char* p1 = s1.c_str();
    p1 = s2.c_str();   // OK

    const char* const p2 = s1.c_str();
    p2 = s2.c_str();   // WRONG
    p2 = nullptr;      // WRONG

    return 0;
}

