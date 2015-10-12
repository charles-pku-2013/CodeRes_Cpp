#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>


using namespace std;
 

void f( vector<int> &v ) 
{
	cout << "f() run in thread." << endl;
	v.clear();
	v.push_back(1);
	v.push_back(3);
	v.push_back(5);
}

struct F {
	F(vector<int> &_v) : v(_v) {}
	
	void operator() ()
	{
		cout << "F()() runs in thread." << endl;
		v.clear();
		v.push_back(2);
		v.push_back(4);
		v.push_back(6);
	}
	
	vector<int> &v;
};
 
 
int main()
{
	vector<int> v1 = {1,2,3,4,5};
	
// 	thread t1(f, std::ref(v1));		//!! 这里必须传入引用

	// 必须使用统一初始化{},否则有编译错误：
	// error: request for member ‘join’ in ‘t1’, which is of non-class type ‘std::thread(F)’
	thread t1{ F{v1} };
	t1.join(); 
	
	copy( v1.begin(), v1.end(), ostream_iterator<int>(cout, " ") );
	cout << endl;
	
	return 0;
}








