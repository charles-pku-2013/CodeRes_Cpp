#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using namespace std;

typedef std::vector< std::vector<int> >         Matrix;

ostream& operator << ( ostream &os, const Matrix &a )
{
    for( int i = 0; i < a.size(); ++i ) {
        copy( a[i].begin(), a[i].end(), ostream_iterator<int>(os, "\t") );
        os << endl;
    }
    return os;
}


bool Find( const Matrix &matrix, int nRows, int nCols, const int &value )
{
    if( nRows <= 0 || nCols <= 0 )
        return false;

	// 从右上角开始找，== 找到，若大于要找的数，排除该列
	// 若小于要找的数，由于右边的列已经被排除，则排除该行
    int r = 0, c = nCols - 1;
    while( r < nRows && c >= 0 ) {
        if( matrix[r][c] == value )
            return true;
        if( matrix[r][c] > value )
            --c;
        else		// <
            ++r;
    } // while

    return false;
}


int main()
{
    int m, n;
    int value;

    cin >> m >> n;
    Matrix arr(m, vector<int>(n, 0));

    for( int i = 0; i < m; ++i ) {
        for( int j = 0; j < n; ++j )
            cin >> arr[i][j];
    } // for

    cout << arr << endl;
    cin >> value;
    cout << value << endl;
    cout << Find( arr, m, n, value ) << endl;

    return 0;
}
