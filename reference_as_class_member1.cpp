#include <cstdio>

struct A {
    int &i;
    // 跟返回局部变量的引用一个道理
    A( int k ) : i(k) {}        // binding reference member 'i' to stack allocated parameter 'k' [-Wdangling-field]
};

int main()
{
    using namespace std;
    
    A a(5);
    printf( "%d\n", a.i );
    a.i = 7;                         // OK ??
    printf( "%d\n", a.i );
    
    return 0;
}