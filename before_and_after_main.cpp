#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Foo {
    Foo() { cout << "Foo construct" << endl; }
    ~Foo() { cout << "Foo destruct" << endl; }
};

static
bool init()
{
    cout << "init called" << endl;
    return true;
}

static bool _dummy_init = init();
static Foo _dummy_foo;

int main()
{
    cout << "in main()" << endl;

    return 0;
}

