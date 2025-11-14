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

template<typename T, typename... Ts, typename U>
void func(T first, Ts... middle, U last) {
    // ...
}
func(10, 20, 30, "hello"); // Error: Ts cannot be deduced
func<int, double, float>(10, 20.0, 30.f, "hello"); // Valid

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
