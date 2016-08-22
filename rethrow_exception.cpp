/*
 * 在 catch block 中跑出的异常，不由本 try...catch... block 处理。
 * 由其上一级处理
 */
#include <iostream>

using namespace std;

class Base
{
public:
    virtual ~Base(){}
};

class Derived : public Base
{
};

void f()
{
    try
    {
        throw Derived();
    }
    catch(Derived& ex)
    {
        cout<<"Caught in f\n";
        throw;
    }

}

int main()
{
    try
    {
        f();
    }
    catch(Base& b)
    {
        cout<<"Caught in main\n";
    }

    return 0;
}
