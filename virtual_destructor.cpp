#include <iostream>
#include <cstdio>

using namespace std;

struct A {
    ~A()
    { cout << "A destructor" << endl; }

    double x;
};

struct B {
    ~B()
    { cout << "B destructor" << endl; }

    double x;
};


struct Base {
    virtual  ~Base()
    { cout << "Base destructor" << endl; }

    A a;
};

struct Derived : Base {
    // Derived 中可以不必显示声明析构函数, B也会析构
    // virtual ~Derived()
    // { cout << "Derived destructor" << endl; }

    B b;  
};
/*
 * B destructor
 * Base destructor
 * A destructor
 */

int main()
{
    Base *p = new Derived;
    delete p;

    return 0;
}
