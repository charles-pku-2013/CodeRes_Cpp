#include <iostream>
#include <set>
#include <iterator>
#include <algorithm>

using namespace std;

struct TestClass {
	TestClass( const int _n ) : n(_n)
	{ cout << "TestClass constructor" << endl; }
	
	int n;
};

void TestFunc( int n )
{
	cout << "TestFunc running..." << endl;
	static TestClass sTest(n);
	cout << "TestFunc ending..." << endl;
}


int main()
{
	cout << "main started..." << endl;
	TestFunc(5);
	TestFunc(6);
	
	return 0;
}
/*
static变量在第一次执行的时候初始化
main started...
TestFunc running...
TestClass constructor
TestFunc ending...
TestFunc running...
TestFunc ending...
 */
