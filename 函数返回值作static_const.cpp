#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;


int add( int a, int b )
{
	printf( "add( %d + %d )\n", a, b );
	return a + b; 
}

// static const inited when first time called
void TestFunc()
{
	printf("TestFunc called\n");
	static const int c = add( 3, 5 );
	printf("c = %d\n", c);
}


int main()
{
	TestFunc();
	TestFunc();
	TestFunc();
	
	return 0;
}


/* 
	TestFunc called
	add( 3 + 5 )
	c = 8
	TestFunc called
	c = 8
	TestFunc called
	c = 8
 */

























