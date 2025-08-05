#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <chrono>

using namespace std;

void test1(int cnt) {
    int sum = 0;
    for (int i = 0; i < cnt; ++i) {
        sum += i;
    }
}

void test2(int i, int j) {
    int result = 1;
    for (int k = 0; k < j; ++k) {
        result *= i;
    }
}

int main() {
    auto timeFuncInvocation =
        [](auto&& func, auto&&... params) {
            auto start = std::chrono::high_resolution_clock::now();

            // NOTE!!! 函数名也需要和参数同样的处理
            std::forward<decltype(func)>(func)(
                std::forward<decltype(params)>(params)...
            );

            auto end = std::chrono::high_resolution_clock::now();
            cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;
        };

    timeFuncInvocation(test1, 1000000);
    timeFuncInvocation(test2, 100, 50);

    return 0;
}

