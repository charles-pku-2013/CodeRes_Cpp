#include <iostream>
#include <random>

using namespace std;

int main() {
    std::random_device            rd;         // obtain a random number from hardware
    std::mt19937                  gen(rd());  // seed the generator
    std::uniform_int_distribution<uint64_t> distr(100, 200);  // define the range 双闭合区间，模板参数默认是int

    for (int i = 0; i < 100; ++i) {
        cout << distr(gen) << " ";
    }
    cout << endl;

    return 0;
}
