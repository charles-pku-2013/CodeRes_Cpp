#include <cstdio>

void func( int (*a)[3] )
{
    
    
}

int main()
{
    int a[3][3] = {
                    {1,2,3},
                    {4,5,6},
                    {7,8,9}
                };
    
//    int b[3] = {10,20,30};
    
//    func(a);
    
    printf( "%x\n", a );
    printf( "%x\n", a+1 );
    printf( "%x\n", a[1] );
    printf( "%x\n", a[1]+1 );
    
    return 0;
}