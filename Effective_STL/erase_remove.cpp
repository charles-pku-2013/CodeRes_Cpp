#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
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
    
    vector<int> a;
    a.reserve(n);
    
    srand( time(0) );
    for( int i = 0; i < n; ++i )
        a.push_back( rand()%10 );
    
    PrintContainer(a);
//    a.erase( remove(a.begin(), a.end(), 0), a.end() );
    a.erase( remove_if(a.begin(), a.end(), bind2nd(less<int>(), 5)), a.end() );
    PrintContainer(a);
    
    return 0;
}




















