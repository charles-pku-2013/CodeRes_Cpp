#include <vector>
#include <string>
#include <iostream>

using namespace std;

void test1()
{
    string s1 = "Hello";
    // string &&ref = s1;   //!! error: rvalue ref cannot bind to lvalue type
    string &&ref = std::move(s1);
    cout << "s1: " << s1 << endl;
    cout << "ref: " << ref << endl;

    // string s2 = ref;    // 这样s1的内容还在, 必须显示使用std::move
    // string s2 = std::move(s1);
    string s2( std::move(ref) );
    cout << "After steal, s1: " << s1 << endl;
}

void test2()
{
    vector< string > strArr;
    string s1 = "Hello";
    //!! C++11 容器鼓励这样使用 rvalue_ref
    strArr.push_back( std::move(s1) );
    cout << "After steal, s1: " << s1 << endl;
    cout << "strArr[0]: " << strArr[0] << endl;
}

vector<string> g_arrString;
void put( const string &s )
{ 
    string &s1 = const_cast<string&>(s);
    g_arrString.push_back(std::move(s1)); // OK, after this, s1 is empty
    // g_arrString.push_back(std::move(s)); 
    // g_arrString.push_back(s);  // test copy on write
}

void test3()
{
    string s1 = "Hello";
    put(s1);
    cout << "After put, s1: " << s1 << endl;
    s1[0] = 'h';   //!! copy on write
}

int main()
{
    // test1();
    // test2();
    test3();
    
    /*
     * string s1;
     * s1.reserve(200);
     * s1.append("Hello").append(" world!").append(" Charles").append( " in PKU." );
     * g_arrString.push_back(std::move(s1));
     */

    return 0;
}
