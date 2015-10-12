#include <iostream>

using namespace std;

template <typename T>
struct Foo {
	Foo( const T &t = T() ) : mem(t) {}
	void bar()
	{ cout << "Foo<T>::bar() in general template." << endl; }
	
	T mem;
};

//!! 模板类成员局部特化 member partial specialization
template<>
void Foo<int>::bar()
{ cout << "Foo<int>::bar() in specialized template." << endl; }


int main()
{
	Foo<char> ch;
	Foo<int> i;
	
	ch.bar();
	i.bar();
 	
	return 0;
}












