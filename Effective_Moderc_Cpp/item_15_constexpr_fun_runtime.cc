#include <iostream>
#include <string>
#include <vector>
// #include <boost/format.hpp>

using namespace std;

constexpr int pow(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

int main() {
    // cout << pow(3, 4) << endl;

    int x, y;
    cin >> x >> y;
    cout << pow(x, y) << endl;  // OK

    return 0;
}

