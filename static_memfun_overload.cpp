#include <iostream>

using namespace std;

struct Base {
    virtual ~Base() = default;
    static bool IsThreadSafe() {
        return true;
    }
};

struct Derived : Base {
    static bool IsThreadSafe() {
        return false;
    }
};

int main() {
    Base *p = new Derived;
    cout << p->IsThreadSafe() << endl;

    return 0;
}

