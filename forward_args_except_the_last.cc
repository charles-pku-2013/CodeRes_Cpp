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
    f(std::forward<Args>(args)..., "world");
}

int main() {
    proxy(1, 2, 3.14);

    return 0;
}
