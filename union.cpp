#include <cstdio>

union A {
    int i;
    char c;
};



int main()
{
    A a;
    printf( "%d\n", sizeof(a) );
    a.i = 0x4241;
    printf( "%c\n", a.c );
    
    return 0;
}