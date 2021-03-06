#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <memory>
#include <set>
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

typedef std::function<bool(const std::string&)>     PredCheck;
void func_as_arg(const std::string &info, const PredCheck& checker = PredCheck())
{
    if (!checker) { 
        cout << "checker not set" << endl; 
        return;
    }

    if (checker(info)) {
        cout << "check pass!" << endl; 
    } else {
        cout << "check not pass!" << endl; 
    }
}

template<typename T>
bool is_in_set(const T &val, const std::set<T> &_set)
{ return _set.count(val) > 0; }

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

    set<string> citySet = {"Beijing", "Shanghai", "Shenzhen"};
    func_as_arg("Beijing", std::bind(is_in_set<string>, std::placeholders::_1, std::ref(citySet)));
	
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











