#include <iostream>
#include <functional>

using namespace std;

struct Foo {
    int add(int a, int b)
    {
        cout << "Foo::add()" << endl;
        return a + b;
    }
};

void test1()
{
    int (Foo::*pf)(int, int) = &Foo::add;
    Foo obj;
    int ret = (obj.*pf)(3, 5);
    cout << ret << endl;
    Foo *ptr = &obj;
    ret = (ptr->*pf)(2, 7);
    cout << ret << endl;
}

void test2()
{
    Foo obj;
    Foo *ptr = &obj;
    std::function<int(int, int)> f = std::bind(&Foo::add, ptr, placeholders::_1, placeholders::_2);
    int ret = f(5, 7);
    cout << ret << endl;
    cout << sizeof(f) << endl;

    std::function<int(int, int)> f1;
    f1 = f;
    if (f1)
        cout << f1(100, 200);
    else
        cout << "empty function object!" << endl;
}


int main()
{
    //test1();
    test2();

	return 0;
}