#include <iostream>
#include <string>
#include <memory>

using namespace std;

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

struct Bar {
    void greet(const std::string &name)
    {
        cout << "Hello " << name << endl;
    }

    void concat(std::string &out, const std::string &s1, const std::string &s2)
    {
        out = s1 + " " + s2;
    }

    int add(int a, int b)
    { return a + b; }
};

template<typename C, typename F, typename... Args>
void call(C&& obj, F&& func, Args&&... args)
{
    (obj.*func)(std::forward<Args>(args)...);
}

/*
 * with return value
 */
// 这个声明必须有
template <typename F>
struct return_type;

// 以下两个 specialization
template <typename R, typename... A>
struct return_type<R(*)(A...)>
{ typedef R type; };

// 这个用于类成员函数
template <typename R, typename C, typename... A>
struct return_type<R(C::*)(A...)>
{ typedef R type; };

// int Add(int a, int b) { return a + b; }

// 需要显示指定return value的类型
template<typename RetType, typename C, typename F, typename... Args>
RetType call2(C&& obj, F&& func, Args&&... args)
{ return (obj.*func)(std::forward<Args>(args)...); }

// 完全自动推导返回类型
template<typename C, typename F, typename... Args>
typename return_type<F>::type call3(C&& obj, F&& func, Args&&... args)
{ return (obj.*func)(std::forward<Args>(args)...); }

int main()
{
    Foo foo;
    call(foo, &Foo::print_sum, 3, 5);
    call(foo, &Foo::print_data);

    auto pBar = std::make_shared<Bar>();
    call(*pBar, &Bar::greet, "beauty"); // NOTE!!! 函数名前必须有取地址算符
    string str;
    call(*pBar, &Bar::concat, str, "hello", "beauty");
    cout << str << endl;
    cout << call2<int>(*pBar, &Bar::add, 5, 4) << endl;   // 可以只指定一个模板参数
    cout << call3(*pBar, &Bar::add, 5, 4) << endl;

    // return_type<decltype(&Bar::add)>::type i = 10;
    // cout << i << endl;

    return 0;
}

