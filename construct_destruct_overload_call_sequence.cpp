#include <iostream>
#include <string>

using namespace std;

class Base {
 public:
    Base() {
        cout << "Base '" << DebugString() << "' construct" << endl;
    }

    virtual ~Base() {
        cout << "Base '" << DebugString() << "' destruct" << endl;
    }

    // NOTE!!! 在构造函数和析构函数里只调用他们自己的成员函数，不会调用继承类重载版本
    virtual std::string DebugString() const {
        return "I am Base";
    }
};

class Derived : public Base {
 public:
    Derived() {
        cout << "Derived '" << DebugString() << "' construct" << endl;
    }

    virtual ~Derived() {
        cout << "Derived '" << DebugString() << "' destruct" << endl;
    }

    std::string DebugString() const override {
        return "I am Derived";
    }
};

int main(int argc, char* argv[]) {
    Base *p = new Derived;
    getchar();
    delete p;

    return 0;
}

#if 0
Base 'I am Base' construct
Derived 'I am Derived' construct

Derived 'I am Derived' destruct
Base 'I am Base' destruct
#endif
