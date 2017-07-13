#include <iostream>

using namespace std;

struct Base {
    Base() { cout << "Base constructor" << endl; }
    virtual ~Base() { cout << "Base destructor" << endl; }
};

struct Derived1 : virtual Base {
    Derived1() { cout << "Derived1 constructor" << endl; }
    virtual ~Derived1() { cout << "Derived1 destructor" << endl; }
};

struct Derived2 : virtual Base {
    Derived2() { cout << "Derived2 constructor" << endl; }
    virtual ~Derived2() { cout << "Derived2 destructor" << endl; }
};

struct Final : Derived1, Derived2 {
    Final() { cout << "Final constructor" << endl; }
    virtual ~Final() { cout << "Final destructor" << endl; }
};


int main()
{
    Base *p = new Final;
    cout << "Processing..." << endl;
    delete p;

    return 0;
}


#if 0
// virtual inheritance
Base constructor            // Base 只构造一次
Derived1 constructor
Derived2 constructor
Final constructor
Processing...
Final destructor    // NOTE!!! 🔴🔵 析构顺序由派生类到基类
Derived2 destructor
Derived1 destructor
Base destructor
#endif


#if 0
// 如果没有virtual
error: ambiguous conversion from derived class 'Final' to base class 'Base':
    struct Final -> struct Derived1 -> struct Base
    struct Final -> struct Derived2 -> struct Base
    Base *p = new Final;
#endif


