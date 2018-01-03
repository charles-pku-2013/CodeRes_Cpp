#include <iostream>
#include <map>
#include <string>

using namespace std;
    
struct Foo {
    Foo(const std::string &_Name) : m_name(_Name) {}

    void greet()
    {
        static int i = 0;   // NOTE!!! i相当于全局变量，不会因对象而不同
        cout << m_name << " greets " << ++i << endl;
    }

    std::string m_name;
};

int main()
{
    Foo a("aaa");
    a.greet();
    a.greet();
    a.greet();

    Foo b("bbb");
    b.greet();
    b.greet();
    b.greet();

    return 0;
}

/*
 * aaa greets 1
 * aaa greets 2
 * aaa greets 3
 * bbb greets 4
 * bbb greets 5
 * bbb greets 6
 */




