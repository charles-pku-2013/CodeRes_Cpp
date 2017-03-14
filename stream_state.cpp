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

// 读入失败会变成0
void test2()
{
    int n = 1, m = 2;
    string s("10a, 21");
    stringstream str(s);
    // str >> n >> m;
    str >> n;  // 遇到'a'结束读入
    cout << "After read n:" << endl;
    print_stream_state(str);
    str >> m;
    cout << "After read m:" << endl;
    print_stream_state(str);
    /*
     * After read n:
     * good: 1
     * eof: 0
     * fail: 0
     * bad: 0
     * 0000
     * After read m:
     * good: 0
     * eof: 0
     * fail: 1
     * bad: 0
     */
    cout << n << endl;
    cout << m << endl;
    print_stream_state( str );
    /*
     * 10
     * 0
     * good: 0
     * eof: 0
     * fail: 1
     * bad: 0
     */
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
    /*
     * 0
     * good: 0
     * eof: 0
     * fail: 1
     * bad: 0
     */
    int n = 10;
    // stringstream str("1234a");
    stringstream str("a1234a");
    str >> n;
    cout << n << endl;
    print_stream_state(str);
}

void test4()
{
    // case 1
    {
        stringstream ss("15 25 35");
        int n = 0;
        ss >> n;
        cout << "n = " << n << endl;
        ss >> n;
        cout << "n = " << n << endl;
        ss >> n;
        cout << "n = " << n << endl;
        print_stream_state(ss);
    }
    // NOTE!!! 最后一次虽然读成功了，但已经是eof
    /*
     * good: 0
     * eof: 1
     * fail: 0
     * bad: 0
     */
    // case 2
    {
        stringstream ss("15 25 35 ");
        int n = 0;
        ss >> n;
        cout << "n = " << n << endl;
        ss >> n;
        cout << "n = " << n << endl;
        ss >> n;
        cout << "n = " << n << endl;
        print_stream_state(ss);
        // 由于末尾多了空格，没有到eof，最后一次读入成功，此时OK
        /*
         * good: 1
         * eof: 0
         * fail: 0
         * bad: 0
         */
        ss >> n;
        cout << "n = " << n << endl;
        print_stream_state(ss);
        // 最后一次读入失败，同时到了文件末尾
        /*
         * n = 35
         * good: 0
         * eof: 1
         * fail: 1
         * bad: 0
         */
    }

    /*
     * while (ss >> n) ...
     * 先读入后判断，适用于以上两种case
     */
}

void test5()
{
    // case 1
    {
        stringstream ss("15 25 35");
        string str;
        str.resize(100, 0);
        // ss.read(&str[0], 100);   // A
        ss.read(&str[0], 8);   // B
        print_stream_state(ss);
        cout << str << endl;
        // A
        /*
         * good: 0
         * eof: 1
         * fail: 1
         * bad: 0
         * 15 25 35 // 读取成功了, 无法获知实际读取了多少字节
         */
        // B 如果stream可以切割成结构化的数据，应该这样读取
        // 适用于 while (ss.read(buf, blocksize)) ...
        /*
         * good: 1
         * eof: 0
         * fail: 0
         * bad: 0
         * 15 25 35
         */
    }

    // case 2
    {
        stringstream ss("15 25 35");
        string str;
        str.resize(100, 0);
        auto sz = ss.readsome(&str[0], 100);
        cout << "Actually read " << sz << " bytes." << endl;
        print_stream_state(ss);
        cout << str << endl;
        // 适用于读取非规则大小stream
        /*
         * good: 1
         * eof: 0
         * fail: 0
         * bad: 0    
         */
    }
}

int main()
{
    // test1();
    // test2();
    // test3();
    // test4();
    test5();

    return 0;
}
