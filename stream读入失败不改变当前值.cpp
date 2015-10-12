#include <cstdio>
#include <iostream>
#include <string>
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
// #include "LOG.h"

using namespace std; 


int main( int argc, char **argv )
{
    string s( "hello world 1 20" );
    stringstream sstr(s);

    string s1, s2;
    sstr >> s1 >> s2;
    cout << s1 << endl << s2 << endl;

    //!! stream 读入失败时候不会改变变量当前值
    int i1 = 5, i2 = 7, i3 = 9;
    sstr >> i1 >> i2 >> i3;
    cout << i1 << endl << i2 << endl << i3 << endl;

    if( !sstr ) {
        cout << "bad stream" << endl;
    } // if

    // cout << sstr.str() << endl;
    
    sstr >> s1 >> s2;
    cout << s1 << endl << s2 << endl;

    return 0;
}
