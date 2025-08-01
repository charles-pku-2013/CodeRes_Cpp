#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

using namespace std;

struct Foo {
    Foo() = default;

    Foo(const Foo& rhs)
    { cout << "Foo copy constructor" << endl; }

    Foo(Foo&& rhs) noexcept  // NOTE!!! 没有 noexcept 会调用 copy constructor
    { cout << "Foo move constructor" << endl; }

    Foo& operator=(const Foo& rhs) {
        cout << "Foo copy assignment" << endl;
        return *this;
    }

    Foo& operator=(Foo&& rhs) {
        cout << "Foo move assignment" << endl;
        return *this;
    }
};

int main(int argc, char **argv) {
    // test1
    {
        std::vector<Foo> arr(10);
        arr.insert(arr.begin(), Foo());
    }

    return 0;
}

