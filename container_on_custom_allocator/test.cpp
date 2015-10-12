#include "alloc.h"
#include <vector>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "LOG.h"

using namespace std;

void* pBaseAddr = 0;

int main()
{
    pBaseAddr = malloc( 100 );
    DBG("addr pBaseAddr = %lx", (long)pBaseAddr);

    vector< int, FixedSizeAllocator<int, 100> > vec;

    vec.push_back( 100 );
    DBG("addr of vec0 = %lx", (long)(&vec[0]));
    DBG_STREAM( "max_size of vec is " << vec.max_size() );

    for( int i = 1; i <= 10; ++i )
        vec.push_back( i );
    DBG("addr of vec0 = %lx", (long)(&vec[0]));

    copy( vec.begin(), vec.end(), ostream_iterator<int>(cout, " ") );
    cout << endl;

    vec.clear();

    for( int i = 1; i <= 100; ++i ) {
        vec.push_back( i );
        DBG("push_back %d element.", i);
    }

    return 0;
}
