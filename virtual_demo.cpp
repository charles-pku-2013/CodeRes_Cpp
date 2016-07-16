#include <iostream>

using namespace std;

struct Base {
    void baseFunc()
    { 
        cout << "BaseFunc()" << endl; 
        allFunc();
    }

    /*
     * NOTE!!! 在基类中调用同一个函数的派生类版本，virtual是必须的。
     * 整个调用链虽然始于派生类指针，但调用了继承自基类的函数baseFunc，
     * 在baseFunc中再调用其他函数，查找规则先本类，再全局，与派生类无关。
     */
    virtual void allFunc()
    { cout << "Base::allFunc()" << endl; }
};

struct Derived : Base {
    void allFunc()
    { cout << "Derived::allFunc()" << endl; }
};


int main()
{
    Derived *pd = new Derived;
    pd->baseFunc();
    
    return 0;
}
