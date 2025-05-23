#include <iostream>
#include <memory>
#include <thread>

using namespace std;

struct Foo {
    Foo()
    { cout << "Foo construct" << endl; }
    ~Foo()
    { cout << "Foo destruct" << endl; }
    void greet() const
    { cout << "Hello" << endl; }
};

void test() {
    cout << "Test begin" << endl;
    static Foo foo;
    foo.greet();
    cout << "Test end" << endl;
}

int main() {
    test();

    cout << "main terminates" << endl;
    return 0;
}

/*
Test begin
Foo construct
Hello
Test end
main terminates
Foo destruct
*/
