#include <iostream>
#include <iterator>
#include <sstream>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;

void print(int x, double y, const std::string& str) {
    cout << x << " " << y << " " << str << endl;
}

int main() {
    auto f = [](auto&&... args) {
        print(std::forward<decltype(args)>(args)...);
    };

    f(10, 3.14, "hello");

    return 0;
}
