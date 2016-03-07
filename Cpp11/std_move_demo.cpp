#include <iostream>
#include <string>
#include <memory>
#include <functional>

using namespace std;

struct Foo {
	Foo( std::string &&_Data ) : data(std::move(_Data)) {}
	std::string data;
};

//!! 什么时候才会发生steal?
// 1. 等号右边(实参)是 std::move(变量名)
// 2. 等号左边(形参)是 Type&&

void func1( string &&arg )
{
    cout << "In func1 arg is " << arg << endl;
    arg = "world";
}


void test()
{
    string s("Hello");
    /*
     * 不可以这样子调用
     * error: no matching function for call to 'func1'
     * 不能从 string 到 string&& 隐式转换
     */
    // func1(s);
    
    // OK
    func1( std::move(s) );
    /*
     * 运行结果：
     * In func1 arg is Hello
     * After calling func1, s is world
     * 当 rvalue ref 仅用作传递参数时，不会发生steal, 和普通non-const ref一样。
     * steal 是由程序员自己实现的，例如，assignment, copy constructor. 
     * 而不是rvalue ref本身特性
     */

    cout << "After calling func1, s is " << s << endl;

    exit(0);
}


void test1()
{
    std::unique_ptr<int> p1(new int(5));
    // std::unique_ptr<int> p2 = p1;  // error
    std::unique_ptr<int> p2 = std::move(p1);

    cout << "p2 is " << *p2 << endl;
    if( !p1 )
        cout << "p1 is empty." << endl;

    exit(0);
}


int main()
{
	string s("Hello");
	// Foo foo(std::move(s));
	// string s1;
	// s1 = std::move(s);
    string s1( std::move(s) );
	
    // Now s is empty, steal happens
	cout << s << endl;

    test();
    // test1();
	
	return 0;
}
