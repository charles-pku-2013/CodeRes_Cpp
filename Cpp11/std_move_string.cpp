#include <iostream>
#include <string>
#include <functional>

using namespace std;

struct Foo {
	Foo( std::string &&_Data ) : data(std::move(_Data)) {}
	std::string data;
};

//!! 什么时候才会发生steal?
// 1. 等号右边(实参)是 std::move(变量名)
// 2. 等号左边(形参)是 Type&&

int main()
{
	string s("Hello");
	// Foo foo(std::move(s));
	string s1;
	s1 = std::move(s);
	
	cout << s << endl;
	
	return 0;
}