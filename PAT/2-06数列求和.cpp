// http://pat.zju.edu.cn/contests/ds/2-06
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

//typedef unsigned char uint8;
typedef unsigned int uint32;
//typedef vector< vector<uint8> > DataSet;


void process( uint32 a, uint32 n )
{
    vector<uint32> vec(n);
    for( uint32 i = 0, len = n; i < n; ++i, --len )
        vec[i] = a*len;
    for( uint32 i = 0; i < n-1; ++i ) {
        uint32 x = vec[i];
        vec[i] = x % 10;
        vec[i+1] += x / 10;
    } // for
    
    reverse( vec.begin(), vec.end() );
    
//    printf( "%u", *(vec.begin()) );
    for( vector<uint32>::iterator it = vec.begin(); it != vec.end(); ++it )
        printf( "%u", *it );
    putchar( '\n' );
}


//void process( uint8 a, uint32 n )
//{
//    DataSet s(n);
//    vector<uint8> result;
//    
//    result.reserve(n+10);
//    for( int i = 0; i < n; ++i )
//        s[i].resize(i+1, a);
//    
//    uint32 sum = 0;
//    for( uint32 i = 0; i < n; ++i ) {
//        for( uint32 j = i; j < n; ++j )
//            sum += s[j][i];
//        result.push_back(sum % 10);
//        sum /= 10;
//    } // for
//    
//    while( sum ) {
//        result.push_back( sum % 10 );
//        sum /= 10;
//    } // while
//    
//    reverse( result.begin(), result.end() );
//    
//    for( vector<uint8>::iterator it = result.begin(); it != result.end(); ++it )
//        printf( "%u", *it );
//    putchar( '\n' );
//}


int main()
{
    uint32 a, n;
    
    while( scanf("%u %u", &a, &n) == 2 ) {
        if( 0 == n ) {
            printf( "0\n" );
            continue;
        } // if
        
        process(a, n);
    } // while
    
    return 0;
}












