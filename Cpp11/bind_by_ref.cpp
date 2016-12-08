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
    foo.data -= n;
}


int main()
{
    using namespace std;

    Foo foo(10);
    /*
     * 源函数 func, 要求2个参数，目标函数f，要求一个int参数
     * bind 本质是调用源函数func，第一个参数foo由bind提供；
     * 第二个参数n由目标函数的第一个参数提供，所以是 placeholders::_1.
     * placeholders 指目标函数的参数位
     */
    /*
     * NOTE!!! 无法达到改变foo值的目的
     * 虽然func接受的是Foo引用参数，但bind生成的对象是含有foo的拷贝而非引用
     * 通过 std::ref 生成 reference_wrapper 对象包装，含有foo的指针。
     */
    // auto f = std::bind(func, foo, std::placeholders::_1);
    auto f = std::bind(func, std::ref(foo), std::placeholders::_1);
    f(20);
    cout << "After calling bind func, foo.data is: " << foo.data << endl;

    return 0;
}


