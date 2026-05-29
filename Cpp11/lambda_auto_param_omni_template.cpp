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

    auto print = [](const auto& arr) {
        for (const auto& item : arr) {
            cout << item << " ";
        }
        cout << endl;
    };

    print(std::vector<int>{1,2,3});

    return 0;
}
