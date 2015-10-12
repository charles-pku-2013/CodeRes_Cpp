#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <ctime>

using namespace std;


template < typename T >
void PrintContainer( const T &c )
{
    typedef typename T::value_type Type;
    copy( c.begin(), c.end(), ostream_iterator<Type>(cout, " ") );
    cout << endl;
}


typedef vector<int>     IntArray;
typedef vector<IntArray>    IntMatrix;

template< typename T >
void Print2DContainer( const T &c )
{
    typedef typename T::value_type          RowType;       //!!
    typedef typename RowType::value_type    ElemType;
    for( typename T::const_iterator it1 = c.begin(); it1 != c.end(); ++it1 ) {
        const RowType &row = *it1;
        copy( row.begin(), row.end(), ostream_iterator<ElemType>(cout, " ") );
        cout << endl;
    } // for
}


int main()
{
    const int n = 5;
    
    IntMatrix m(n, IntArray(n, 0));
    Print2DContainer(m);
    
    srand( time(0) );
    for( int i = 0; i < n; ++i ) {
        for( int j = 0; j < n; ++j )
            m[i][j] = rand()%10;
    } // for
    
    Print2DContainer(m);
    
    return 0;
}




















