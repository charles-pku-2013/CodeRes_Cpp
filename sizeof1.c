#include <stdio.h>

typedef struct X {
    char a;             // 4
    int b;              // 4
    short c;            // 2
    char d;             // 6
    long e;             // 8
} XX;


int main()
{
    int *p = NULL;
    XX x = {1, 2, 3, 4, 5};
    
    printf( "%d\n", sizeof(long) );
    printf( "%d\n", sizeof(short) );
    printf( "%d\n", sizeof(int) );
    printf( "%d\n", sizeof(p) );
    printf( "%d\n", sizeof(*p) );
    printf( "%d\n", sizeof(XX) );
    
    putchar( '\n' );
    printf( "%x\n", &x );
    printf( "%x\n", &x.a );
    printf( "%x\n", &x.b );
    printf( "%x\n", &x.c );
    printf( "%x\n", &x.d );
    printf( "%x\n", &x.e );
    
    putchar( '\n' );
    printf( "%d\n", x.a );
    printf( "%d\n", x.b );
    printf( "%d\n", x.c );
    printf( "%d\n", x.d );
    printf( "%d\n", x.e );
    return 0;
}













