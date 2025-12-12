#include <fmt/base.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

#include <iostream>
#include <optional>
#include <string>
#include <thread>  // 用于模拟耗时操作 std::this_thread::sleep_for
#include <utility>

using Array = std::vector<int>;

using namespace std;

// 当你使用 std::forward<Array>(arr) 时，Array 不是一个引用类型。
// 根据 std::forward 的工作原理，当模板参数 T 不是左值引用类型时，
// std::forward<T>(arg) 会将 arg 转换回一个右值引用（Array&&）。
struct Foo {
    explicit Foo(Array arr)
        : arr_(std::forward<Array>(arr))  // NOTE 这里std::forward效果等同于std::move
                                          // 当你使用 std::forward<Array>(arr) 时，Array 不是一个引用类型。
                                          // 根据 std::forward 的工作原理，当模板参数 T 不是左值引用类型时，
                                          // std::forward<T>(arg) 会将 arg 转换回一个右值引用（Array&&）。
                                          // 因此，this->arr_(std::forward<Array>(arr)) 这一行实际上调用了 arr_ 成员变量的移动构造函数。
    {
        cout << fmt::format("Foo construct {:x}", (uint64_t)(arr_.data())) << endl;
    }

    Array arr_;
};

int main() {
    Array arr{1, 2, 3, 4};
    cout << fmt::format("main {:x}", (uint64_t)(arr.data())) << endl;
    Foo foo(std::move(arr));

    return 0;
}
