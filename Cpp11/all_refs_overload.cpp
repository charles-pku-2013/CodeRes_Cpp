#include <iostream>
#include <string>

using namespace std;

struct Foo {
    int x, y, z;
    std::string name;
};

void func(Foo &f)
{ cout << "ref" << endl; }

void func(const Foo &f)
{ cout << "const ref" << endl; }

void func(Foo &&f)
{ cout << "rvalue ref" << endl; }

int main() {
    Foo foo;
    // func(foo); // ref
    func(std::move(foo));

    return 0;
}

