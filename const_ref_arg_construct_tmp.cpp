#include <iostream>
#include <string>

using namespace std;

struct Foo {
    Foo(int _N) : n(_N)
    { cout << "Foo constructor" << endl; }
    ~Foo()
    { cout << "Foo destructor" << endl; }

    int n;
};


void test1( const std::string &str )
{
    string &s = const_cast<string&>(str);
    s = "shit";
    cout << str << endl;
}

// NOTE!!! call implicit constructor to build a tmp var
void test2( const Foo &f )
{
    cout << f.n << endl;
}
/*
 * Foo constructor
 * 5
 * Foo destructor
 */

int main()
{
    // test1("hello, world!");
    test2(5);

    return 0;
}

