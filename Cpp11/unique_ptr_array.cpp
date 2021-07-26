#include <memory>
#include <iostream>

using namespace std;

void test() {
    struct Foo {
        Foo() { cout << "Foo construct" << endl; }
        ~Foo() { cout << "Foo destruct" << endl; }
    };

    // NOTE!!! 自动调用数组版的delete
    std::unique_ptr<Foo[]> ptr(new Foo[10]);
}

int main()
{
    test();

    return 0;
}

