#include <cstdio>
#include <cstring>
#include <string>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;

bool flag;

// suppose must have '1'
void Permutation( char *s, const int n, const int i, int &j, const int k )
{
//    cout << "Permutation " << " j=" << j << " k= " << k << endl;
    if( i == n ) {
        if( j == k ) {
//            cout << "j == k" << endl;
            flag = false;
            cout << s << endl;
        } // if
        return;
    }
    if( !flag ) return;
    
    char *p = strchr(s, '1');           // first 1
    for( char *q = p; q >= s; --q ) {
        if( q != p ) {
            swap( *p, *q );
            ++j;
//            printf( "j=%d\n", j );
        } // if
        Permutation(s, n, i+1, j, k);
        if( q != p ) swap( *p, *q );
    } // for
}


int main()
{
    int t;
    int n, m, k;
    scanf( "%d\n", &t );
    while( t-- ) {
        scanf( "%d %d %d\n", &n, &m, &k );
        string str;
        str.append( n, '0' );
        str.append( m, '1' );
        cout << str << endl;
        char *s = const_cast<char*>(str.c_str());
        flag = true;
        int j = 1;
        Permutation( s, str.length(), 0, j, k );
        if( flag ) cout << "Impossible" << endl;
    } // while
    
    return 0;
}












