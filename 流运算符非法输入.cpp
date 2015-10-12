#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <deque>
#include <cmath>
#include <cassert>
#include <exception>
// #include "LOG.h"

using namespace std; 

//!! 流运算符遇到第一个非法输入就终止
int main( int argc, char **argv )
{
    string s("1, 2, 3, 4, 5");
    // string s("1 2 3 4 5");
    stringstream str(s);
    
    vector<int> v;
    copy( istream_iterator<int>(str), istream_iterator<int>(), back_inserter(v) );

    copy( v.begin(), v.end(), ostream_iterator<int>(cout, " ") );
    cout << endl;

    return 0;
}
