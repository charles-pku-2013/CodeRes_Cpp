#include <cstdio>
#include <cstring>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>


using namespace std;


//!! 纯虚函数可以有实现，但必须在类定义外部实现。

class Base {
public:
    virtual ~Base() = 0; 
    virtual void print() const = 0;
};

inline Base::~Base()
{ cout << "Base destructor" << endl; }

inline void Base::print() const     //!! const 在声明和定义中都不可以省略
{ cout << "I'm base" << endl; }


class Derived : public Base {
public:
    ~Derived()
    { cout << "Derived destructor" << endl; }

    void print() const
    {
        Base::print(); 
        cout << "I'm derived" << endl; 
    }
};


int main()
{
    Base *p = new Derived;
    p->print();
    delete p;

/*
 * I'm base
 * I'm derived
 * Derived destructor
 * Base destructor
 */

    return 0;
}
