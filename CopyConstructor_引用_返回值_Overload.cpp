#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>


struct Test {
	Test() : data(0) { printf("Test Default Constructor %lx\n", (long)this); }
	
	Test( const Test &rhs )
	{ printf("Test Copy Constructor %lx = %lx\n", (long)this, (long)&rhs); }
	
	Test& operator= ( const Test &rhs )
	{
		printf("Test operator = %lx = %lx\n", (long)this, (long)&rhs);
		return *this; 
	}
	
	int data;
};

struct Wrapper {
	Test m;
	Test& func()
	{ 
		return m; 
	}
};


struct Wrapper1 {
	Test m;
	Test func()
	{ 
		return m; 
	}
};


Test test_func()
{
	Test t;
	t.data = 5;
	return t;
}


int main()
{
	// Wrapper w;
//	Test t = w.func();	// one copy constructor
	// Test &t = w.func();	// most saving, no copy constructor called
	
//	Wrapper1 w;
//	Test t = w.func();	// one copy constructor, same as above
//	Test &t = w.func();	// error

	Test t = test_func();	// only constructor called, optimizition made.
	printf( "%d\n", t.data );
	
	return 0;
}

























