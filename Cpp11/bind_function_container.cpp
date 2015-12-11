#include <iostream>
#include <functional>
#include <list>


void print_add( int a, int b )
{
    std::cout << a+b << std::endl;
}

//!! 不可以用 auto 做函数参数
template <typename CallType>
void func( CallType f )
{
    f();
}
 
struct Foo {
    Foo( int _Data ) : data(_Data) {}

    void print_data() const
    {
        std::cout << data << std::endl;
    }

    int data;
};


int main()
{
    using namespace std;

    auto obj = std::bind( print_add, 5, 3 );
    func( std::move(obj) );
    // cout << sizeof(obj) << endl;
    // obj();
    
    typedef std::function<void()>       CallbackType;
    CallbackType f = std::bind( print_add, 10, 20 );

    Foo foo(25);
    CallbackType f1 = std::bind( &Foo::print_data, &foo );
    // cout << sizeof(f) << endl;
    // cout << sizeof(f1) << endl;

    std::list<CallbackType> joblist;
    joblist.push_back( std::move(f) );
    joblist.push_back( std::move(f1) );

    while( !joblist.empty() ) {
        joblist.front()();
        joblist.pop_front(); 
    }

    return 0;
}


