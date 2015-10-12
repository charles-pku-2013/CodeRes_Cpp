#include <iostream>
#include <cstdio>


using namespace std;

void printSet( int *a, int n )
{
    for( int *p = a; p < a+n; ++p ) {
        if( *p )
            printf("%d", *p);
    }
    putchar('\n');
}


void subset( int *a, int n, int level )
{
    if( level == n ) {
        printSet(a, n);
        return;
    }
    
    int old = a[level];
    a[level] = 0;                   // 左分支排除该元素
    subset( a, n, level+1 );
    a[level] = old;                 // restore
    subset(a, n, level+1 );         // 右分支包含该元素
}



int main()
{
	int a[]={1, 2, 3};
	subset(a, sizeof(a)/sizeof(*a), 0);
    
    return 0;
}

