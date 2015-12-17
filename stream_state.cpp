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

/*
  fail: 可恢复错误
  bad: 不可恢复的严重错误
  当用ifs.read读完文件时，good = 0, eof = 1, fail = 1, bad = 0
  此时若想判断 fstream 状态，应该用 if(!ifs.bad()) 或
  if( !ifs ) {
	  if( ifs.eof() )
		  // 当success处理
	  else // fail
  }
*/


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
