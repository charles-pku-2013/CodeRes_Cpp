#include <iostream>
#include <utility>  // For std::forward

struct Foo {
};

void test(Foo&& foo) {
}

int main() {
    Foo foo;
    test(std::move(foo));

    return 0;
}
