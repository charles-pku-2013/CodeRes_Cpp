#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>
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
    
    list<int> a;
//    a.reserve(n);
    
    srand( time(0) );
    for( int i = 0; i < n; ++i )
        a.push_back( rand()%10 );
    
    PrintContainer(a);
//    a.remove(0);
    a.remove_if( bind2nd(less<int>(), 5) );
    PrintContainer(a);
    
    return 0;
}




















