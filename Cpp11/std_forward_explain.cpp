#include <iostream>
#include <utility>

using namespace std;


/*
void func2( int &&j )
{
	cout << j << endl;
}

void func1( int &&i )
{
	//!! named rvalue reference is lvalue
	func2(i);	// error: cannot bind ‘int’ lvalue to ‘int&&’
}
 */


/*
// std::forward implement
template<typename T>                // For lvalues (T is T&),
T&& std::forward(T&& param)         // take/return lvalue refs.
{                                   // For rvalues (T is T),
    return static_cast<T&&>(param); // take/return rvalue refs.
}
 */

void f( int v1, int &v2 )
{
	cout << "In f(): " << v1 << " " << ++v2 << endl;
}

void g( int &&i, int &j )
{
	cout << "In g(): " << i << " " << j << endl;
}

template < typename F, typename T1, typename T2 >
void flip( F f, T1 t1, T2 t2 )
{
	f( t2, t1 );
}


template < typename F, typename T1, typename T2 >
void flip2( F fp, T1 &&t1, T2 &&t2 )
{
	//!! named rvalue ref is lvalue
	fp( t2, t1 );	//!! here t2 is lvalue.
}


template < typename F, typename T1, typename T2 >
void flip3( F fp, T1 &&t1, T2 &&t2 )
{
	fp( std::forward<T2>(t2), std::forward<T1>(t1) ); //!! std::forward<> template arg must be provided
}


int main()
{
/*
	int i = 5;
	func1(i);
 */

	int i = 5;
	f(42, i);
	cout << "In main(): i = " << i << endl;

	int j = 10;
	//!! Even through f() v2 is ref, j passed to flip by value, change in f cannot reflect back.
	// flip( f, j, 42 );
	// flip2( g, j, 42 );	// error: cannot bind ‘int’ lvalue to ‘int&&’
	flip3( g, j, 42 );
	cout << "In main(): j = " << j << endl;

	return 0;
}












