#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <cmath>
#include <ctime>
#include <climits>
#include <sstream>
#include <algorithm>
#include <functional>
#include <utility>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <deque>
#include <cassert>
#include <exception>
#include <bitset>
// #include "LOG.h"

// http://hihocoder.com/problemset/problem/1105?sid=556871
// 堆操作 push_heap pop_heap


/*
 * 描述
 * 
 * 小Ho有一个糖果盒子，每过一段时间小Ho都会将新买来的糖果放进去，同时他也会不断的从其中挑选出最大的糖果出来吃掉，但是寻找最大的糖果不是一件非常简单的事情，所以小Ho希望能够用计算机来他帮忙计算这个问题！
 * 
 * 提示：吃糖果吃多了会变胖的！
 * 
 * 输入
 * 
 * 每个测试点（输入文件）有且仅有一组测试数据。
 * 
 * 在一组测试数据中：
 * 
 * 第1行为1个整数N，表示需要处理的事件数目。
 * 
 * 接下来的M行，每行描述一个事件，且事件类型由该行的第一个字符表示，如果为'A'，表示小Ho将一粒糖果放进了盒子，且接下来为一个整数W，表示这颗糖果的重量；如果为'T'，表示小Ho需要知道当前盒子中最重的糖果的重量是多少，在知道这个值之后，小Ho会将这颗糖果从盒子中取出并吃掉。
 * 
 * 对于100%的数据，满足1<=N<=10^5, 1<=w<=10^5。<>
 * 
 * 对于100%的数据，满足没有2颗糖果的重量是相同的，最开始的时候小Ho的糖果盒子是空的，且每次小Ho想要取出一颗糖果的时候盒子里一定至少有一颗糖果。
 * 
 * 输出
 * 
 * 在一组测试数据中：
 * 
 * 对于每个类型为'T'的时间，输出1个整数W_MAX，表示在这一时刻，盒子中最重的糖果的重量。
 * 
 * 
 * 
 * 样例输入
 * 5
 * A 77751
 * A 1329
 * A 26239
 * A 80317
 * T
 * 样例输出
 * 80317
 */

using namespace std; 

vector<int> heap;

int main( int argc, char **argv )
{
    int n;
    string line;
    char event;
    int w;

    cin >> n;
    getline( cin, line );
    while( n-- ) {
        getline( cin, line );
        stringstream str(line);
        str >> event;
        if( event == 'A' ) {
            str >> w;
            heap.push_back( w );
            std::push_heap( heap.begin(), heap.end() );
        } else if( event == 'T' ) {
            std::pop_heap( heap.begin(), heap.end() );
            cout << heap.back() << endl;
            heap.pop_back();
        } // if 
    } // while 

    return 0;
}
