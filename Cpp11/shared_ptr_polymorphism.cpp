#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <memory>

using namespace std;

struct Base {
    Base( int _X ) : x(_X) {}
    virtual ~Base() { cout << "Base Destructor" << endl; }
    virtual void print() const { cout << "x = " << x << endl; }
    int x;
};

struct Derived : Base {
    Derived( int _X, int _Y ) : Base(_X), y(_Y) {}
    ~Derived() { cout << "Derived destructor" << endl; }
    void print() const { cout << "x = " << x << " y = " << y << endl; }
    int y;
};

int main()
{
    std::shared_ptr<Base> p(new Derived(3,5));
    p->print();
    
    return 0;
}













