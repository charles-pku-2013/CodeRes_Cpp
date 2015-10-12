#include <vector>
#include <functional>
#include <cstdio>


using namespace std;



struct Test {
    double a, b, c, d;
};

struct TestStruct {
    TestStruct( Test &_r ) : r(_r) {}
    Test &r;
};


int main()
{
    printf( "%ld\n", sizeof(Test) );
    printf( "%ld\n", sizeof(TestStruct) );
    Test t;
    TestStruct tr(t);
    printf( "%ld\n", sizeof(tr) );
    Test &r = t;
    printf( "%ld\n", sizeof(r) );
    return 0;
}



/*
 32
 8
 8
 32
 */
















