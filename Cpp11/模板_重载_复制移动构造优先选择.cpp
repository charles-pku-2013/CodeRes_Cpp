#include <iostream>
#include <utility>

using namespace std;

/* //!! error: call of overloaded ‘tfunc(int)’ is ambiguous
template < typename T >
void tfunc( T i )
{ cout << "T " << i << endl; }
 */
 
template < typename T >
void tfunc( T &&i )
{ cout << "T && " << i << endl; }

template < typename T >
void tfunc( const T &i )
{ cout << "const T & " << i << endl; }


void func( int &&i )
{ cout << "int && " << i << endl; }

void func( const int &i )
{ cout << "const int & " << i << endl; }


template <typename T>
struct Foo {
	Foo() {}
	Foo( const T& _obj ) : obj(_obj) {}
	Foo( const Foo<T> &rhs ) : obj(rhs.obj)
	{
		cout << "Foo Copy Constructor" << endl;
	}
	
	Foo( Foo<T> &&rhs ) : obj(rhs.obj)
	{	
		rhs.obj = 0;
		cout << "Foo move constructor" << endl;
	}
	
	T obj;
};


int main()
{
	func(5);		// int && is prefered
	
	int i = 6;
	func(i);		// const int & is prefered //!! that's explain why copy constructor is prefered over move constructor.
					//!! 解释为什么复制构造函数优先于移动构造函数
	
	tfunc(5);		// T&&
	tfunc(i);		// T&& //!! 和上面的普通函数不一样
	
	Foo<int> f1(5);
	Foo<int> f2(f1); // Copy Constructor prefered
	Foo<int> f3( std::move(f1) );	// use move constructor
 	
	return 0;
}












