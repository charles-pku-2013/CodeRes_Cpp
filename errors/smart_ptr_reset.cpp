#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct Foo {
    Foo(const std::string &_Info) : m_info(_Info) 
    { cout << "Foo constructor " << m_info << endl; }

    ~Foo() { cout << "Foo destructor " << m_info << endl; }

    std::string m_info;
};


int main()
{
    std::shared_ptr<Foo> pFoo(new Foo("aaa"));

    // pFoo.reset();    // NOTE!!! 全局唯一资源只能有一个，需要显示释放 delete 旧的
    pFoo.reset(new Foo("bbb")); // NOTE!!! 最基本的常识，先计算表达式，作为参数传给函数，这都忘了!!!

    return 0;
}

