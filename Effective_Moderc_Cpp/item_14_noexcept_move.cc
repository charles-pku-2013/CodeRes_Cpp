#include <iostream>
#include <string>
#include <vector>
// #include <boost/format.hpp>

using namespace std;

struct Foo {
    Foo() {
        cout << "Foo default construct" << endl;
    }

    Foo(const Foo& rhs) {
        cout << "Foo copy construct" << endl;
    }

    /*
     * NOTE! by using noexcept, move construct will be used instead of copy construct
     */
    Foo(Foo&& rhs) noexcept {
        cout << "Foo move construct" << endl;
    }

    // char buf[1024];
};

int main() {
    std::vector<Foo> arr;
    arr.resize(5);
    arr.resize(10);

    return 0;
}

