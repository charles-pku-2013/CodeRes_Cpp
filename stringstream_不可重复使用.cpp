#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include "LOG.h"

using namespace std; 

#define DBG(...)

void ch2n( int &n )
{
    n -= '0';
}

class Solution {
public:
    vector<int> plusOne(vector<int>& digits) 
    {
        stringstream sstr;
        copy( digits.begin(), digits.end(), ostream_iterator<int>(sstr, "") );
        
        unsigned long val;
        sstr >> val;
        ++val;
        DBG("%d", val);

        //!! 不可重复使用stringstream
        stringstream sstr1;
        sstr1 << val;
        string s = sstr1.str();
        DBG( "%s", s.c_str() );

        vector<int> retval( s.begin(), s.end() );
        for_each( retval.begin(), retval.end(), ch2n );

        return retval;
    }
};


void test()
{
    stringstream sstr;
    sstr << 556;

    int val;
    sstr >> val;
    cout << val << endl;

    sstr << "Hello";
    string s = sstr.str();
    cout << s << endl;

    exit(0);
}


int main( int argc, char **argv )
{
    test();

    int a[] = {9,8,7,6,5,4,3,2,1,0};
    vector<int> vec( a, a+sizeof(a)/sizeof(*a) );

    Solution sln;
    vec = sln.plusOne( vec );
    copy( vec.begin(), vec.end(), ostream_iterator<int>(cout, " ") );
    cout << endl;

    return 0;
}
