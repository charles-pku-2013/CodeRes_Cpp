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