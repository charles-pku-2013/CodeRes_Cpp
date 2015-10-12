#include <iostream>
#include <string>
#include <utility>
#include <typeinfo>
#include <cstdio>

using namespace std;

/*
 * T 本身是引用，所以返回类型用这种形式，先得到type（值类型），再加上&&，否则本身是引用可能会返回 “& &&” 类型
	根据 reference collapse 原则，其实是 & 类型
 * T is a universal reference: rvalue, lvalue, const, non-const, etc...
   Conditions:
   1. T is a template type. (function template, not class template)
   2. Type deduction (reference collapsing happened to T)
*/
template < typename T >
typename remove_reference<T>::type&& MyMove( T&& arg )
{
	if( is_same< T, typename remove_reference<T>::type >() )
		cout << "T is value type"  << endl;
	else if( is_same< T, typename remove_reference<T>::type& >() )
		cout << "T is ref type&"  << endl;
	else if( is_same< T, typename remove_reference<T>::type&& >() )
		cout << "T is rvalue ref type&&"  << endl;
	else
		cout << "Unknown type T" << endl;
	return static_cast< typename remove_reference<T>::type&& >( arg );
}

/* 
//!! 用模板特化实现 remove_reference
template< class T > struct remove_reference      {typedef T type;};
template< class T > struct remove_reference<T&>  {typedef T type;};
template< class T > struct remove_reference<T&&> {typedef T type;};
 */
 
int main()
{	
	// test 1
	// {
		// int &&r1 = MyMove(5);		// value
		// MyMove(r1);					// ref // named rvalue ref as lvalue ?
		
		// int i = 7;
		// MyMove(i);					// ref
		
		// const int n = 10;
		// MyMove(n);				// ref
		
		// MyMove( "Hello" );
		// MyMove( string() );
	// }
	
	// test 2
	{
		int i = 5;
		const int &r1 = i;
		const int &&r2 = MyMove(r1);
	}
	
	return 0;
}















