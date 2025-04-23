#include <iostream>
#include <vector>

struct Foo {
    const std::vector<int>& getData() const
    { return data_;  }

    std::vector<int> data_;
};

int main() {
    Foo foo;

    static_assert(
        std::is_same_v<std::decay<decltype(foo.getData())>::type, std::vector<int>>
    );

    return 0;
}

