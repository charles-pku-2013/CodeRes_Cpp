#include <iostream>

using namespace std;

void print_int( /* const */ int &i ) 
{ cout << "lvalue reference " << i << endl; }

void print_int( int &&i )
{ cout << "rvalue reference " << i << endl; }

/* 
void print_int( int i )
{ cout << "value " << i << endl; }	//!! cannot overload, overload only based on reference
 */

int main()
{
	int i = 5;
	
	print_int(i);	// lvalue ref
	print_int(6);	// rvalue ref
	
	return 0;
}



