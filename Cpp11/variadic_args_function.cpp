#include <iostream>
#include <ctime>
#include <string>

using namespace std;

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

int main()
{
    call_func(add, 3, 5);
    call_func(print_time);
    call_func(print_string, "Hello, world!");

    return 0;
}
