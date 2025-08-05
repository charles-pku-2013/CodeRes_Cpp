#include <iostream>
#include <vector>

using namespace std;

struct Foo {
    Foo() = default;

    Foo(const Foo& rhs) {
        cout << "Foo copy constructor" << endl;
    }

    /*
     * NOTE with `noexcept` move construct will be called on vector insert
     * without `noexcept` copy construct is called
     */
    Foo(Foo&& rhs) noexcept {
        cout << "Foo move constructor" << endl;
    }

    // error: object of type 'Foo' cannot be assigned because its copy assignment operator is implicitly deleted
    Foo& operator=(const Foo& rhs) {
        cout << "Foo copy assignment" << endl;
        return *this;
    }

    Foo& operator=(Foo&& rhs) noexcept {
        cout << "Foo move assignment" << endl;
        return *this;
    }
};

int main(int argc, char **argv) {
    std::vector<Foo> arr(10);
    arr.insert(arr.begin(), Foo());

    return 0;
}

