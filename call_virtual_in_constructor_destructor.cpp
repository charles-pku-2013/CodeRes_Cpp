#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace std;

struct Base {
    Base()
    { 
        cout << "Base constructor." << endl;
        print(); //!! 在构造函数中调用虚函数不会调用派生类的版本
    }

    virtual ~Base()
    { 
        cout << "Base destructor." << endl; 
        print(); //!! 在析构函数中调用虚函数不会调用派生类的版本
    }

    virtual void print()
    { cout << "Hi, Base." << endl; }
};


struct Derived : public Base {
    Derived()
    { cout << "Derived constructor." << endl; }
    ~Derived()
    { cout << "Derived destructor." << endl; }

    void print()
    { cout << "Hi, Derived." << endl; }
};


int main()
{
    std::shared_ptr<Base> p = std::make_shared<Derived>();
    return 0;
}

/*
 * Base constructor.
 * Hi, Base.
 * Derived constructor.
 * Derived destructor.
 * Base destructor.
 * Hi, Base.
 */
