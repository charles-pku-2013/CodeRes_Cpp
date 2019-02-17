#include <iostream>
#include <memory>

using namespace std;

struct Foo;
std::unique_ptr<Foo> g_pFoo;

struct Foo {
    Foo() {
        cout << "Foo constructor" << endl;
        if (g_pFoo)
            cout << "global ptr is set" << endl;
        else
            cout << "global ptr is not set" << endl;
    }
};

int main()
{
    g_pFoo.reset(new Foo);

    return 0;
}

