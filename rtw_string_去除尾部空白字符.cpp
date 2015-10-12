#include <string>
#include <iostream>

// 现在理解函数内常亮名要复杂一些的原因了
#define SPACES       " \t\f\r\v\n"

using namespace std;


void rtw_string( std::string &s )
{
    static const char *__RTW_SPACES__ = " \t\f\r\v\n";

    string::size_type pos = s.find_last_not_of( __RTW_SPACES__ );

    if( pos != string::npos )
        s.resize( pos + 1 );
}


int main()
{
    string s( "Hello world!         " );

    rtw_string( s );
    cout << s << endl;
    cout << s.length() << endl;

    return 0;
}
