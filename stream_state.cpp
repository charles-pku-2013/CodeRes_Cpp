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


void test1()
{
    vector<int> vec;
    copy( istream_iterator<int>(cin), istream_iterator<int>(), back_inserter(vec) );
    print_stream_state( cin );
}

void test2()
{
    int n, m;
    string s("10a, 21");
    stringstream str(s);
    str >> n >> m;
    cout << n << endl;
    print_stream_state( str );
}

// 遇到非法字符的表现
void test3()
{
    /*
     * if (str) 表示还可以继续读入
     * 如果正常读入结束，只有eof是true，注意，此时good也是false
     * 一般情况如果读入失败因为格式问题，fail置为1，
     * bad只有在类似系统错误的严重情况下才为1
     */
    int n;
    // stringstream str("1234a");
    stringstream str("a1234a");
    str >> n;
    cout << n << endl;
    print_stream_state(str);
}

int main()
{
    // test1();
    // test2();
    test3();

    return 0;
}
