#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <iterator>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <bitset>

using namespace std;

struct Foo {
    Foo( const string &_Name ) : name(_Name)
    {
        printf("Foo obj %s constructed.\n", name.c_str());
    }

    ~Foo()
    {
        printf("Foo obj %s destructed.\n", name.c_str());
    }

    string name;
};

/*
 * 构造顺序只与定义顺序有关，与初始化列表顺序无关。
 * 析构顺序恰好与构造顺序相反。
 */

struct Container {
    Container( const string &s1, const string &s2 )
            : obj2(s2)
            , obj1(s1)
    {}

    Foo obj1;
    Foo obj2;
};
/*
 * Foo obj obj1 constructed.
 * Foo obj obj2 constructed.
 * Foo obj obj2 destructed.
 * Foo obj obj1 destructed.
 */


struct A {
    A() : n2(1), n1(n2 + 2) {}
    int     n1;
    int     n2;
};


int main()
{
    // test 1
    /*
     * {
     *     Container *p = new Container("obj1", "obj2");
     *     getchar();
     *     delete p;
     * }
     */

    // test 2 剑指Offer p244，再次证明，成员变量初始化顺序只与定义顺序有关
    {
        A a;
        cout << "a.n1 = " << a.n1 << endl;
        cout << "a.n2 = " << a.n2 << endl;
        // test result:
        // a.n1 = 2
        // a.n2 = 1
    }

    return 0;
}
