#include <iostream>
#include <map>
#include <thread>
#include "tbb/tbb.h"
#include "tbb/parallel_for.h"

#define THIS_THREAD_ID      std::this_thread::get_id()
#define SLEEP_SECONDS(x)    std::this_thread::sleep_for(std::chrono::seconds(x))

using namespace tbb;
using std::cout; using std::endl;
 
void Foo( int x )
{
    cout << "Thread " << THIS_THREAD_ID << " got value " << x << endl;
    SLEEP_SECONDS(1);
    cout << "Thread " << THIS_THREAD_ID << " Done!" << endl;
}

void ParallelApplyFoo( int a[], size_t n ) 
{
    parallel_for(0, n, [&](int i) {
        Foo(a[i]);
    } );
}

int main( int argc, char **argv )
{
    int a[] = {1,2,3,4,5,6,7,8,9,10};
    ParallelApplyFoo( a, sizeof(a) / sizeof(*a) );

    return 0;
}

