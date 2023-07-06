#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <utility>
#include "absl/strings/str_format.h"

class Foo {
 public:
    Foo() {
        std::cerr << absl::StrFormat("Foo construct %lx", (uint64_t)this) << std::endl;
    }
    Foo(const Foo &r) {
        std::cerr << absl::StrFormat("Foo copy construct %lx from %lx", (uint64_t)this, (uint64_t)&r) << std::endl;
    }
    Foo(Foo &&r) {
        std::cerr << absl::StrFormat("Foo move construct %lx from %lx", (uint64_t)this, (uint64_t)&r) << std::endl;
    }
    int m = 0;
};

void greet(const std::vector<Foo> &arr) {
    std::cerr << "hello " << arr.size() << std::endl;
}

void test1(Foo&& foo) {
    std::cerr << absl::StrFormat("addr of foo is %lx", (uint64_t)&foo) << std::endl;
}

template<typename T>
void test(T&& arg) {
    // test1(std::forward<T>(arg));
    std::cerr << absl::StrFormat("addr of arg is %lx", (uint64_t)&arg) << std::endl;
    arg.m = 10;
}

int main() {
    // std::vector<Foo> arr(10);
    // std::cerr << absl::StrFormat("addr of arr is %lx", (uint64_t)(arr.data())) << std::endl;

    // auto f = [a = std::move(arr)]() {
        // std::cerr << absl::StrFormat("addr of a is %lx", (uint64_t)(a.data())) << std::endl;
        // greet(a);
    // };

    // f();

    // std::cerr << "arr size is " << arr.size() << std::endl;

    Foo foo;
    // test(std::move(foo));
    test(foo);
    std::cerr << foo.m << std::endl;

    return 0;
}

