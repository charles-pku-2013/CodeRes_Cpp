#include <iostream>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <cctype>
#include <algorithm>

void ToUpper( char &ch )
{
    ch = toupper(ch);
}


int main()
{
    using namespace std;

    string s("abcdefg");
    cout << s << endl;
//    for_each( s.begin(), s.end(), ToUpper );
    transform( s.begin(), s.end(), s.begin(), ::toupper );
    cout << s << endl;
    

    return 0;
}







