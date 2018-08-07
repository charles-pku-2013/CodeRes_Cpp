#include <iostream>
#include <memory>

using namespace std;

struct Base {
    virtual ~Base() = default;

    virtual void operator()()
    { cout << "This is Base." << endl; }
};

struct Derived : Base {
    virtual void operator()()
    { cout << "This is Derived." << endl; }  // call this
};

int main()
{
    std::shared_ptr<Base> p = std::make_shared<Derived>();

    (*p)();

    return 0;
}
