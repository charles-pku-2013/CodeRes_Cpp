#include <functional>
#include <iostream>
 
using namespace std;

struct Foo {
    Foo() : m_nData(0) {}

    void add(int n)
    { m_nData += n; }

    int m_nData;
};

int main()
{
    Foo a;
    // auto f = std::bind(&Foo::add, &a, 5);   // right 5 Recommended
    // auto f = std::bind(&Foo::add, a, 5);   // WRONG 0
    auto f = std::bind(&Foo::add, std::ref(a), 5);   // right 5
    f();

    cout << a.m_nData << endl;
	
	return 0;
}
