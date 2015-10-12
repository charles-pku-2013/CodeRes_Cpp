#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

struct false_type {
	operator bool() const { return false; }
};

struct true_type {
	operator bool() const { return true; }
};


template < typename T >
struct is_void : false_type {};

template<>  // explicit specialization for T = void
struct is_void<void> : true_type {};


int main()
{
    // for any type T other than void, the 
    // class is derived from false_type
    if( is_void<char>() )
		cout << "char is void type" << endl;
	else
		cout << "char is not void type" << endl;
    // but when T is void, the class is derived
    // from true_type
    if( is_void<void>() )
		cout << "void is void type" << endl;
	else
		cout << "void is not void type" << endl;  
	
	return 0;
}



























