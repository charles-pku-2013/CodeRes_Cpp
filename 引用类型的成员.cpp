#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <ctime>

using namespace std;


struct Test {
    Test( int &_x ) : x(_x) {}      //!! 形参必须是引用，否则x将是 dangling reference
    int &x;
    
    void print() const
    { printf( "Test::print %lx\n", &x ); }
};


int main()
{
    int i = 5;
    Test t(i);
    
    printf( "%lx\n", &i );
    t.print();
    
    return 0;
}




















