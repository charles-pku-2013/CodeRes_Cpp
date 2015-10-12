#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>

using namespace std;

// template<class T, T v>
// struct IntegralConstant {
    // static const T value = v;
    // typedef T value_type;
    // typedef IntegralConstant type;
    // operator value_type() const { return value; }
	// value_type operator()() const { return value; }
// };


template <typename T, T value>
struct IntegralConstant {
    typedef T value_type;
    typedef IntegralConstant type;
    operator value_type() const { return value; }	//!! 编译器会自动调用类型转换函数，尽可能匹配，如果使用了被包装对象的成员函数
	// value_type operator()() const { return value; }
};


bool strless( const char *s1, const char *s2 )
{ return strcasecmp(s1, s2) < 0; }


//!! 直接这样定义是错误的
// template <typename T value>
// struct Test {
	// operator T() const { return value; }
// };


int main()
{	
	// test 1
	// {
		// typedef IntegralConstant<int, 5> Five;
		
		// Five a;
		
		// cout << a << endl;
		// printf( "%d\n", (int)a );	//!! by virtue of operator value_type()
	// }
	
	// test 2
	{
		typedef IntegralConstant< bool(*)(const char*, const char*), strless > CompareType;
		// CompareType a;
		// set<const char*, CompareType> strSet;
		// strSet.insert( "bbb" );
		// strSet.insert( "AAA" );
		
		// for( set<const char*, CompareType>::iterator it = strSet.begin(); it != strSet.end(); ++it )
			// cout << *it << endl;
		
		CompareType cmp;
		bool ret = cmp( "aaa", "BBB" );
		cout << ret << endl;
	}
	
	return 0;
}















