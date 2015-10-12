#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <ctime>

using namespace std;


template < typename T >
void PrintContainer( const T &c )
{
    typedef typename T::value_type Type;
    copy( c.begin(), c.end(), ostream_iterator<Type>(cout, " ") );
    cout << endl;
}


void EraseVector( vector<int> &a )
{
    for( vector<int>::iterator it = a.begin(); it != a.end(); ) {
        if( (*it) % 3 == 0 ) {
            it = a.erase(it);           //!!
        } else {
            ++it;
        } // if
    } // for
}



struct Predicate : public unary_function<int, bool> {
    bool operator() ( const int n ) const
    { return (n % 3 == 0); }
};

void RemoveCopyIf( vector<int> &a )
{
    vector<int> tmp;
    remove_copy_if( a.begin(), a.end(), back_inserter(tmp), Predicate() );
    a.swap(tmp);
}


int main()
{
    const int n = 10;
    
    vector<int> a;

    for( int i = 0; i < n; ++i )
        a.push_back( i+1 );
    
    PrintContainer(a);
    
//    EraseVector(a);
    RemoveCopyIf(a);

    PrintContainer(a);
    
    return 0;
}




















