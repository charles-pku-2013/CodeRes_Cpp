#include <cstdio>
#include <vector>

using namespace std;

struct TestClass {
    TestClass( int i ) : index(i)
    { printf("TestClass Constructor %d\n", index); }
    
    TestClass( const TestClass &rhs )
    { index = rhs.index; printf("TestClass Copy Constructor %d\n", index); }
    
    TestClass& operator= ( const TestClass &rhs )
    {
        index = rhs.index;
        printf("TestClass = %d\n", index);
        return *this;
    }
    
    ~TestClass()
    { printf("TestClass Destructor %d\n", index); }
    
    int index;
    char buf[1024];
};


int main()
{
    vector<TestClass> vec;
    
    for( int i = 0; i < 10; ++i )
        vec.push_back( TestClass(i) );
    
    printf( "Program Ending .......\n" );
    
    return 0;
}
