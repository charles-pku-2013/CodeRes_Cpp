#include <random>
#include <iostream>
#include <memory>
#include <functional>
#include <thread>
 
struct Foo {
    void print_sum(int n1, int n2)
    {
        std::cout << n1+n2 << '\n';
    }

    void print_data()
    {
        std::cout << data << std::endl;
    }

    int data = 10;
};
 
int main()
{
    using namespace std::placeholders;  // for _1, _2, _3...
 
    // bind to a member function
    Foo foo;
    // auto f3 = std::bind(&Foo::print_sum, &foo, 95, _1);
    // f3(5);

    // auto f = std::bind(&Foo::print_data, &foo);
    // f();
    
    std::thread thr1( std::bind(&Foo::print_data, &foo) );
    thr1.join();
 
    std::thread thr2( std::bind(&Foo::print_sum, &foo, 95, _1), 5 );
    thr2.join();

    return 0;
}
