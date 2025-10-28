#include <iostream>
#include <vector>
#include <array>
#include <iterator>

using namespace std;

void f(int a, int b, float c, const std::string& d) {
    cout << a << b << c << d << endl;
}

template<typename... Args>
void proxy(Args&&... args) {
    f(std::forward<Args>(args)..., "world");  // OK
}

// WRONG
// template<typename... Args>
// void proxy2(Args&&... args, const std::string& s) {
    // f(std::forward<Args>(args)..., s);
// }

template<typename... Args>
void proxy3(int i, Args&&... args) {
    f(i, std::forward<Args>(args)...);
}

int main() {
    proxy(1, 2, 3.14);
    // proxy2(1, 2, 3.14, "hi");
    proxy3(1, 2, 3.14, "hi");

    return 0;
}
