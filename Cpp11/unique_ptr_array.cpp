#include <iostream>
#include <string>
#include <memory>

struct Foo {
    Foo() {
        std::cout << "Foo default construct" << std::endl;
    }

    Foo(int i) : data_(i) {
        std::cout << "Foo construct data=" << data_ << std::endl;
    }

    ~Foo() {
        std::cout << "Foo destruct data=" << data_ << std::endl;
    }

    int data_ = 0;
};


int main() {
    std::unique_ptr<Foo[]> ptr(new Foo[10]{[0 ... 9] = {5}});

    return 0;
}
