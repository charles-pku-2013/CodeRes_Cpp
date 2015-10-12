#include <iostream>
#include <cstring>
#include <cstdlib>
#include <algorithm>

using namespace std;


void AllPermution( char *s, const int n, const int i )
{
    if( i == n ) {
        cout << s << endl;
        return;
    } // if
    
    for( int j = i; j < n; ++j ) {
        if( j != i ) swap( s[i], s[j] );
        AllPermution( s, n, i+1 );
        if( j != i ) swap( s[i], s[j] );        //!! restore 回朔必须的，否则会有重复
    } // for
}



int main()
{
    char str[] = "1234";
    AllPermution( str, strlen(str), 0 );
    
//    swap( str[0], str[1] );
//    cout << str << endl;
    
    return 0;
}




















