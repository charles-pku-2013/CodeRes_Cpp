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
#include <exception>
#include <stdexcept>

using namespace std;

struct Foo {
    Foo( const string &_Name ) : name(_Name)
    {
        fprintf(stderr, "Foo obj %s constructed.\n", name.c_str());
    }

    ~Foo()
    {
        fprintf(stderr, "Foo obj %s destructed.\n", name.c_str());
    }

    string name;
};


struct Container {
    Container( const string &s1, const string &s2 )
    {
        //!! 构造函数失败抛出异常时候，需要显示释放哪些要求显示释放的资源
        // 这不是自动指针
        //!! 构造函数抛出异常，该类Container的析构函数不会被执行。
        // 这也是为什么有些类设计成构造函数什么也不做而用init成员函数来做初始化。
        obj1 = new Foo( s1 );
        throw std::runtime_error("test exception");
        obj2 = new Foo( s2 );
    }

    ~Container()
    {
        delete obj1;
        delete obj2;
    }

    Foo *obj1;
    Foo *obj2;
};



void func()
{
    Foo obj1("func_obj1");
    throw std::runtime_error( "exception in func" );
    Foo obj2("func_obj2");
}

int main()
{
    // Container *p = new Container("obj1", "obj2");

    // getchar();

    // delete p;
    
    // try {
        // func();
    // } catch ( const std::exception &ex ) {
        // cerr << ex.what() << endl;
    // }

    try {
        Container c("obj1", "obj2");
    } catch ( const std::exception &ex ) {
        cerr << ex.what() << endl;
    }

    return 0;
}
