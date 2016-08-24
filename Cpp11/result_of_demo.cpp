/*
 * Deduces the return type of a function call expression at compile time. 
 */
#include <type_traits>
#include <iostream>
 
struct S {
    double operator()(char, int&);
    float operator()(int) { return 1.0;}
};
 
template<class T>
typename std::result_of<T(int)>::type f(T& t)
{
    std::cout << "overload of f for callable T\n";
    return t(0);
}
 
template<class T, class U>
int f(U u)
{
    std::cout << "overload of f for non-callable T\n";
    return u;
}
 
int main()
{
    // the result of invoking S with char and int& arguments is double
    std::result_of<S(char, int&)>::type d = 3.14; // d has type double
    static_assert(std::is_same<decltype(d), double>::value, "");
 
    // the result of invoking S with int argument is float
    std::result_of<S(int)>::type x = 3.14; // f has type float
    static_assert(std::is_same<decltype(x), float>::value, "");
 
    // result_of can be used with a pointer to member function as follows
    struct C { double Func(char, int&); };
    std::result_of<decltype(&C::Func)(C, char, int&)>::type g = 3.14;
    static_assert(std::is_same<decltype(g), double>::value, "");
 
    f<C>(1); // may fail to compile in C++11; calls the non-callable overload in C++14

    return 0;
}
