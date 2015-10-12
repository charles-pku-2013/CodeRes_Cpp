#include <iostream>
#include <cstring>
#include <cstdlib>
#include <set>
#include <algorithm>
#include <functional>
#include <ctime>

using namespace std;


template < typename T >
void PrintContainer( const T &c )
{
    typedef typename T::value_type Type;
    copy( c.begin(), c.end(), ostream_iterator<Type>(cout, " ") );
    cout << endl;
}



int main()
{
    const int n = 10;
    
    set<int> a;

    for( int i = 0; i < n; ++i )
        a.insert( i+1 );
    
    PrintContainer(a);
    
    for( set<int>::iterator it = a.begin(); it != a.end(); ) {
        if( (*it) % 3 == 0 ) {
            a.erase( it++ );
        } else {
            ++it;
        } // if
    } // for

    PrintContainer(a);
    
    return 0;
}




















