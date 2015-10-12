#include <cstdio>


#define DBG(fmt, ... ) fprintf( stdout, fmt"\n", ##__VA_ARGS__ )


template<typename T>
struct FuncPtrDemo {
    typedef int (T::*FuncPtr)( int );
    
    FuncPtrDemo( T *_obj, FuncPtr _ptr ) : obj(_obj), ptr(_ptr) {}
    
    void testFuncPtr( int x )
    {
        (obj->*ptr)(x);
    }
    
    FuncPtr ptr;
    T *obj;
};


struct Demo {
    int test( int x )
    {
        DBG( "Base::test(%d)", x );
        return x;
    }
};


int main()
{
    using namespace std;
    
    
    Demo *p = new Demo;
    
    DBG( "test 1" );
    typedef int (Demo::*FuncPtr)( int );
    FuncPtr fp = &Demo::test;
    (p->*fp)(5);
    
    DBG( "test 2" );
    FuncPtrDemo<Demo> d( p, &Demo::test);
    d.testFuncPtr(10);
    
    return 0;
}




























