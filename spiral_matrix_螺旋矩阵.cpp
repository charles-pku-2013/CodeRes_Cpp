#include <cstdio>
#include <cstring>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>

using namespace std;

struct Matrix : std::vector< std::vector<int> > {
    typedef std::vector< std::vector<int> > BaseType;
    Matrix( int n ) : BaseType(n, std::vector<int>(n, 0)) {}
};


ostream& operator << ( ostream &os, const Matrix &m )
{
    for( size_t i = 0; i < m.size(); ++i ) {
        copy( m[i].begin(), m[i].end(), ostream_iterator<int>(os, "\t") );
        os << endl;
    }

    return os;
}


static int counter = 0;
void FillRoundMatrix( Matrix &m, const int n, const int k )
{
    if( k > n/2+1 ) return;

    //!! 第一行必须是全的，否则最中心元素无法循环到
    // up
    for( int i = k; i < n-k; ++i )
        m[k][i] = ++counter;
    // right
    for( int i = k+1; i < n-k-1; ++i )
        m[i][n-k-1] = ++counter; 
    // down
    for( int i = n-1-k; i >= k+1; --i )
        m[n-1-k][i] = ++counter;
    // left
    for( int i = n-1-k; i >= k+1; --i )
        m[i][k] = ++counter; 

    FillRoundMatrix( m, n, k+1 );
}


int main()
{
    const int n = 7;
    Matrix m(n);

    FillRoundMatrix(m, n, 0);

    cout << m << endl;

	return 0;
}


