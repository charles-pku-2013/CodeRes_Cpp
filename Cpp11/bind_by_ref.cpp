#include <iostream>
#include <functional>


struct Foo {
    Foo( int _Data ) : data(_Data) {}

    Foo( const Foo &rhs )
    {
        data = rhs.data;
        std::cout << "Foo Copy Constructor " << data << std::endl;
    }

    void print_data() const
    {
        std::cout << data << std::endl;
    }

    int data;
};

void func( Foo &foo, int n )
{
    std::cout << n + foo.data << std::endl;
}


int main()
{
    using namespace std;

    Foo foo(10);
    // auto f = std::bind(func, foo, std::placeholders::_1);
    auto f = std::bind(func, std::ref(foo), std::placeholders::_1);
    f(20);

    return 0;
}


