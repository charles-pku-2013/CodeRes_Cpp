#include <stdio.h>
#include <limits.h>
#include <float.h>


int main()
{
    printf( "CHAR_MIN %x\n", CHAR_MIN );
    printf( "CHAR_MAX %x\n", CHAR_MAX );
    printf( "UCHAR_MAX %x\n", UCHAR_MAX );
    printf( "INT_MIN %x\n", INT_MIN );
    printf( "INT_MAX %x\n", INT_MAX );
//    printf( "%x\n", UINT_MIN );           // UMIN === 0
    printf( "UINT_MAX %x\n", UINT_MAX );
    printf( "LONG_MIN %lx\n", LONG_MIN );
    printf( "LONG_MAX %lx\n", LONG_MAX );
    printf( "ULONG_MAX %lx\n", ULONG_MAX );
    printf( "SHORT_MIN %x\n", SHRT_MIN );
    printf( "SHORT_MAX %x\n", SHRT_MAX );       // SHRT not SHORT
    printf( "USHORT_MAX %x\n", USHRT_MAX );
    
//    printf( "FLT_MIN %f\n", FLT_MIN );
    
    return 0;
}














