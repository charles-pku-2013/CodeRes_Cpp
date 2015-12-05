#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <iterator>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <vector>
#include <bitset>

using namespace std;

template< typename StreamType >
void print_stream_state( const StreamType &stream )
{
    cout << "good: " << stream.good() << endl;
    cout << "eof: " << stream.eof() << endl;
    cout << "fail: " << stream.fail() << endl;
    cout << "bad: " << stream.bad() << endl;
    cout << bitset<4>(stream.rdstate()).to_string() << endl;
}

int main()
{
    // test 1
    /*
     * {
     *     vector<int> vec;
     *     copy( istream_iterator<int>(cin), istream_iterator<int>(), back_inserter(vec) );
     *     print_stream_state( cin );
     * }
     */

    // test 2
    {
        int n, m;
        string s("10a, 21");
        stringstream str(s);
        str >> n >> m;
        cout << n << endl;
        print_stream_state( str );
    }

    return 0;
}
