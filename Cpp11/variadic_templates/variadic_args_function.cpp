#include <iostream>
#include <ctime>
#include <string>

using namespace std;

// 类模板可变模板参数应放在最后

template <typename Func, typename... Args>
void call_func( Func &&f, Args&&... args )
{
    // cout << "In call func()" << endl;
    f(args...);
}

int add( int a, int b )
{
    int c = a + b;
    cout << a << " + " << b << " = " << c << endl;
    return c;
}

void print_time()
{
    time_t now = time(0);
    cout << ctime(&now) << endl;
}

void print_string( const std::string &s )
{
    cout << s << endl;
}

// 全部参数都可以是variadic args，与C里面的va_arg区别
template <typename... Args>
void test(Args&&... args)
{
    auto ret = add(args...);
    cout << ret << endl;
}

template <typename... Args>
void sizeof_test(Args&&... args)
{
    size_t sz = sizeof...(args); // sizeof...(Args) 两者等价
    cout << sz << endl;
}

int main()
{
    call_func(add, 3, 5);
    call_func(print_time);  // zero arg
    call_func(print_string, "Hello, world!");
    test(3, 5);
    sizeof_test(3, 5, 'a', "hello", 3.14);

    return 0;
}
