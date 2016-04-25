#include <iostream>
#include <string>

using namespace std;

static
void test1()
{
    // total heap usage: 1 allocs, 1 frees, 30 bytes allocated
    struct Foo {
        Foo( const string &_Name ) : name(_Name) {}

        string name;
    };

    Foo f("Hello");
}

static
void test2()
{
    struct Foo {
        Foo( const string &_Name ) : name(_Name) {}

        string name;
    };

    string s("Hello");
    // total heap usage: 1 allocs, 1 frees, 30 bytes allocated
    Foo f(s);
    // total heap usage: 2 allocs, 2 frees, 60 bytes allocated
    s[0] = 'h';
}

int main()
{
    // test1();
    test2();

    return 0;
}
