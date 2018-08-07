#include <memory>
#include <iostream>
#include <boost/format.hpp>

using namespace std;

template<typename T>
void delete_arr(T *p)
{ delete [] p; }

struct Foo {
    Foo() { cout << "Foo constructor " << boost::format("%lx") % (void*)this << endl; }
    ~Foo() { cout << "Foo denstructor " << boost::format("%lx") % (void*)this << endl; }
};

int main()
{
    std::shared_ptr<Foo> ptr(new Foo[10], delete_arr<Foo>);
    // std::shared_ptr<Foo> ptr(new Foo[10]);

    return 0;
}

