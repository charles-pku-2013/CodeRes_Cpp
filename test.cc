#include <iostream>

struct Foo {
    static int x;
};

int Foo::x = 5;

int main() {
    Foo foo;
    (&foo)->x = 10;
    std::cout << foo.x << std::endl;

    return 0;
}
