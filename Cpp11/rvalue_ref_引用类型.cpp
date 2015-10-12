#include <iostream>
#include <string>
#include <utility>
#include <cstdio>


struct String : std::basic_string<char> {
	typedef std::basic_string<char>		BaseType;
	
	String() = default;
	
	String( const char *s ) : BaseType(s)
	{ printf("String constructor @%lx: %s\n", (long)(this->c_str()), this->c_str()); }
	
	String( const String &rhs ) : BaseType(rhs)
	{ printf("String copy constructor @%lx: %s\n", (long)(this->c_str()), this->c_str()); }
	
	String( String &&rhs ) : BaseType(rhs)
	{ printf("String move constructor @%lx: %s\n", (long)(this->c_str()), this->c_str()); }
	
	String& operator= ( const String &rhs )
	{
		printf( "copy @%lx %s <- %s\n", (long)(this->c_str()), this->c_str(), rhs.c_str() );
		BaseType::operator=(rhs);
		printf( "copy @%lx %s <- %s\n", (long)(this->c_str()), this->c_str(), rhs.c_str() );
		return *this;
	}	
	
	String& operator= ( String &&rhs )
	{
		printf( "move @%lx %s <- %s\n", (long)(this->c_str()), this->c_str(), rhs.c_str() );
		BaseType::operator=(rhs);
		printf( "move @%lx %s <- %s\n", (long)(this->c_str()), this->c_str(), rhs.c_str() );
		return *this;
	}
	
	~String()
	{ printf("String destructor @%lx: %s\n", (long)(this->c_str()), this->c_str()); }
};


void func( int &&r )
{ std::cout << r++ << std::endl; }


int main()
{
	using namespace std;
	
	// test 1
	// {
		// int &&r1 = 5;
		// cout << ++r1 << endl;	// OK
		// int &&r2 = r1;	//!! ERROR named rvalue refs are treated as lvalue, and rvalue ref cannot bind to lvalue
		// int &&r2 = static_cast<int&&>(r1);	// OK, this is how std::move() implemented
	// }
	
	// test 2
	// {
		// const int i = 5;
		//int &&r1 = i;	//!! error! const int expr is not a rvalue
		// int &&r1 = std::move(i);	// errorr! same as above
	// }
	
	// test 3
	// {
		// int i = 5;
		//int &&r1 = i;	// error! i is a lvalue
		// int &&r1 = std::move(i);	// OK
		// cout << ++r1 << endl;
		// cout << i << endl;	// i is changed to 6
	// }
	
	// test 4
	// {
		// string &&r1 = "Hello C++";
		// r1[0] = 'h';	// hello C++
		// cout << r1 << endl;
	// }
	
	// test 5
 	// {
		// String &&r1 = "Hello, C++";	// constructor and destructor called
		// r1 = "ISO C++ 11";
// /* 		char str[] = "Hello, C++";
		// printf("addr of const char string is %lx\n", (long)(&str[0]));
		// String &&r1 = std::move(str);
		// printf("addr of String is %lx\n", (long)(r1.c_str())); */
		
		// /* 		
		// addr of const char string is 7fff334541b0		// in data segment
		// String constructor: Hello, C++
		// addr of String is 1ac7028						// in heap
		// String destructor: Hello, C++
		 // */
		// /* we can see that a temporary String is constructed */
	// } 
	
	// test 6
/* 	{
		int i = 5;
		// func(i);	// ERROR rvalue ref cannot bind to a lvalue
		func( std::move(i) );
	} */
	
	// test 7
	// {
		// int i = 5;
		// int &r1 = i;
		// int &&r2 = static_cast<int&&>(r1);	// OK This is how std::move() implemented.
		/* lvalue ref canbe static_cast into rvalue ref */
	// }
	
	// test 8
	// {
		// String s1 = "Hello, C++";
		// String s2 = std::move(s1);
	// }
	
	// test 9
	// {
		// String s1;
		// s1 = "Hello";	// move operator =
		
		// String s2;
		// s2 = s1;
		// printf( "before change, addr of s2: %lx %lx\n", (long)(s2.c_str()), (long)(&s2[0]) );
		// s2[0] = 'x';
		// printf( "after change, addr of s2: %lx %lx\n", (long)(s2.c_str()), (long)(&s2[0]) );
		// cout << s1 << " " << s1.c_str() << endl;
		// cout << s2 << " " << s2.c_str() << endl;
	// }
	
	// test 10 //!! static_cast can convert a lvalue to rvalue ref
	{
		int i = 5;
		int &&r = static_cast<int&&>(i);
		++r;
		cout << i << endl;		
	}
	
	return 0;
}


/* 
output of test 8
String constructor @1c49028: Hello, C++
String move constructor @1c49028: Hello, C++
String destructor @1c49028: Hello, C++
String destructor @1c49028: Hello, C++
 */













