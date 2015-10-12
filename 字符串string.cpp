#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

//!! string 即使空串其指针也指向一个内容为0的内存，底层指针永不为0

int main()
{
	// test 1
/* 	{
		string s;
		
		printf( "%lx\n", (long)(s.data()) );
		cout << s.empty() << endl;
		cout << s.length() << endl;
		cout << s.size() << endl;
		cout << ( *(s.data()) == 0 ) << endl;
	} */
	
	// test 2
 	{
		// C字符串可以这样连接起来，不需要运行时申请内存
		//!! 注意指向常量串的指针用const
		// #define PATH "NodeTopic"
		// const char *s = "/"PATH;
		const char *s = "Namespace/" "MyTopic";
		cout << s << endl;
	} 
	
	// test 3 string rstrip
/* 	{
		const char *SPACES = " \t\f\r\v\n";
		// string s("");		// i is npos
		// string s("abc");		// i points to last char 'c'
		// string s("abc    ");	// i points to last char 'c'
		string s("     ");		// i is npos	//!! 空串和空白串都是 npos
		string::size_type i = s.find_last_not_of( SPACES );
		cout << i << endl;
		cout << ( i == string::npos ) << endl;
	} */
	
	// test 4 strtok
/* 	{
		const char *SPACES = " \t\f\r\v\n";
		// char str[] = "";		// p is NULL
		char str[] = "     ";	// p is NULL
		char *p = strtok( str, SPACES );
		cout << ( p == NULL ) << endl;
	} */
	
	return 0;
}

/* 
test1
7f38ebad43f8
1
0
0
1
test2
/NodeTopic
 */

























