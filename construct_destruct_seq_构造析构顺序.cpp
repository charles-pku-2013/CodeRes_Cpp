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

#define BUFSIZE          (1024)

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


int main()
{
    Container *p = new Container("obj1", "obj2");

    getchar();

    delete p;

    return 0;
}
