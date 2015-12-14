#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <memory>
#include <thread>
#include <functional>

using namespace std;

void func( const string &s )
{ cout << s << endl; }


struct Foo {
	Foo( int _Data ) : data(_Data) {}
	void print_data() const
	{ cout << data << endl; }
	
	void print_msg( const std::string &msg )
	{ cout << msg << endl; }
	
	int data;
};


int main()
{
    std::function<void(const std::string&)> f;
    f = func;
	cout << sizeof(f) << endl;
    if( f )
        f( "hello" );
	
	Foo foo(10);
	std::function<void(void)> f1 = std::bind( &Foo::print_data, &foo );
	cout << sizeof(f1) << endl;
	f1();
    
	string msg("Hello");
	std::function<void(void)> f2 = std::bind( &Foo::print_msg, &foo, msg );
	cout << sizeof(f2) << endl;
	f2();
	
    return 0;
}

/*
 * 32
 * hello
 * 32
 * 10
 * 32
 * Hello
 */











