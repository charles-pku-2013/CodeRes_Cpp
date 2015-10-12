#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

bool valid( int *a, int n )
{
    if( n == 1 )
        return true;
    for( int *p = a+1; p < a+n; ++p ) {
        if( *p < *(p-1) )
            return false;
    } // for
    
    return true;
}


int compute( int *a, int n )
{
    for( int len = n / 2; len > 1; --len ) {
        for( int *start = a; (start+len) < (a+n); ++start ) {
            if( !valid(start, len) )
                continue;
            int *ret = search( start+len, a+n, start, start+len );
            if( ret != (a+n) )
                return len;
        } // for
    } // for
    
    return 0;
}


int main()
{
    int len, res;
    
    scanf( "%d\n", &len );
    vector<int> vec(len);
    for( int i = 0; i < len; ++i )
        scanf( "%d", &vec[i] );
    
    res = compute( &vec[0], len );
    
    printf("%d\n", res);
    
    return 0;
}














