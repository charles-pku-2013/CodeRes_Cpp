#include <iostream>

using namespace std;

class Base {
public:
    Base() {
        foo();
    }

    virtual ~Base() {
        foo();
    }

    virtual void foo() {
        cout << "Base foo" << endl;
    }
};

class Derived : public Base {
public:
    Derived() {
        foo();
    }

    ~Derived() {
        foo();
    }

    void foo() override {
        cout << "Derived foo" << endl;
    }
};


int main()
{
    Base *p = new Derived;
    cout << "\nprocessing...\n" << endl;
    delete p;

    return 0;
}

#if 0
Base foo
Derived foo

processing...

Derived foo
Base foo
#endif
