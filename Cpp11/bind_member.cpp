#include <iostream>
#include <functional>
#include <vector>

using namespace std;

void fill_arr(std::vector<int> &arr, int n) {
    arr.clear();
    for (int i = 1; i <= n; ++i)
        arr.emplace_back(i);
}

struct Foo {
    void func() {
        auto f = std::bind(&fill_arr, std::ref(arr_), 10);
        f();
        for (auto &i : arr_)
            cout << i << endl;
    }

    std::vector<int> arr_;
};


int main() {
    Foo f;
    f.func();

    return 0;
}
