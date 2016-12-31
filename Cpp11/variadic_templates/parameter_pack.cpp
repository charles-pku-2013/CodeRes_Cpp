#include <iostream>
#include <ctime>
#include <string>
#include <tuple>

// Parameter pack
// http://en.cppreference.com/w/cpp/language/parameter_pack

using namespace std;

template <typename... Args>
struct Foo {
    Foo(Args&&... args) : m_tuple(std::make_tuple(std::forward<Args>(args)...)) {}

    std::tuple<Args...>     m_tuple;
};


template <typename T, typename... Args>
struct Foo1 {
    Foo1(const T& arg1, Args&&... args) : m_tuple(std::make_tuple(std::forward<Args>(args)...)) {}

    std::tuple<Args...>     m_tuple;
};


// 按引用传递，但不能传指针，cannot convert from const char* to const char*&
template <typename... Args>
std::tuple<Args...> test(Args&&... args)
{
    return std::make_tuple(std::forward<Args>(args)...);
}

// 按值传递就可以处理指针
template <typename... Args>
std::tuple<Args...> test2(Args... args)
{
    return std::make_tuple(args...);
}

// 按const引用传递也可以处理指针
template <typename... Args>
std::tuple<Args...> test3(const Args&... args)
{
    return std::make_tuple(args...);
}

int main()
{
    const char *str = "Hello";
    test3<int, char, const char*, double>(1, 'a', str, 3.14);
    test(1, 2, 3, 4.2, 'a');

    // Foo<int, char, const char*, double> foo(1, 'a', str, 3.14);  // wrong
    Foo1<int, char, const char*, double> foo(1, 'a', "Hello", 3.14);    // OK

    return 0;
}
