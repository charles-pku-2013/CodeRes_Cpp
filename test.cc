#include <iostream>
#include <thread>

#define THIS_THREAD_ID        std::this_thread::get_id()

using namespace std;

class Base1 {
public:
    virtual void func1() { std::cout << "Base1::func1" << std::endl; }
};
 
class Base2 {
public:
    virtual void func2() { std::cout << "Base2::func2" << std::endl; }
};
 
class Derived : public Base1, public Base2 {
public:
    virtual void func1() { std::cout << "Derived::func1" << std::endl; }
    virtual void func3() { std::cout << "Derived::func3" << std::endl; }
};

int main() {
    Base1 *p = new Derived;
    p->func1();
    // p->func2();
    // p->func3();

    return 0;
}
