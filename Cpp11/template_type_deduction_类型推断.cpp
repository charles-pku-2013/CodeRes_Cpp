#include <iostream>
#include <utility>

using namespace std;

//!! 若以lvalue调用，形参若是 T 和 T&， T都是value type， //!!形参若是T&&，T是ref type&
//!! 若以rvalue调用， 形参是 T 或 T&&， T都是value
template < typename T >
void func( T &&i )
{ 
	if( is_same< T, typename remove_reference<T>::type >() )
		cout << "T is value type"  << endl;
	else if( is_same< T, typename remove_reference<T>::type& >() )
		cout << "T is ref type&"  << endl;
	else if( is_same< T, typename remove_reference<T>::type&& >() )
		cout << "T is rvalue ref type&&"  << endl;
	else
		cout << "Unknown type T" << endl;
	
	++i; 
}


int main()
{
	int i = 5;
	func(i);
	
	cout << i << endl;
	
	return 0;
}



