#include <stdio.h>

// 12
struct A {
    int d;      // 4
    short a;    // 2
    short b;    // 2
    short c;    // 4
};

// 6
struct B {
    short a;    // 2
    short b;    // 2
    short c;    // 2
};

// 8
struct C {
    int a;    // 4
    short b;    // 4
};

int main()
{
    struct A a = {1, 2, 3};
    printf( "%ld\n", sizeof(struct A) );
    printf( "%ld\n", sizeof(struct B) );
    printf( "%ld\n", sizeof(struct C) );
    printf( "%lx\n", &a );
    printf( "%lx\n", &a.d );
    printf( "%lx\n", &a.a );
    printf( "%lx\n", &a.b );
    printf( "%lx\n", &a.c );
    
    return 0;
}













