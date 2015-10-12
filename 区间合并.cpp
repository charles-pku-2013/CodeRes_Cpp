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


struct Interval {
    // Interval() {}
    Interval( int s, int e ) : start(s), end(e) {}
    int start, end;
};

struct IntervalCmp {
    bool operator() ( const Interval &lhs, const Interval &rhs ) const
    { return lhs.start < rhs.start; }
};

ostream& operator << ( ostream &os, const Interval &i )
{
    os << "[" << i.start << "," << i.end << "]";
    return os;
}

istream& operator >> ( istream &in, Interval &i )
{
    in >> i.start >> i.end;
    return in;
}

bool is_in_range( int start, int end, int p )
{
    return ( p >= start && p <= end );
}

bool merge_range( const Interval &lhs, const Interval &rhs, Interval &new_range )
{
    bool meragable = is_in_range( lhs.start, lhs.end, rhs.start );

    if( meragable ) {
        new_range.start = lhs.start;
        new_range.end = ( lhs.end > rhs.end ? lhs.end : rhs.end );
    } // if 

    return meragable;
}

class Solution {
public:
    vector<Interval> merge(vector<Interval> &intervals) 
    {
        if( intervals.size() <= 1 )
            return intervals;

        sort( intervals.begin(), intervals.end(), IntervalCmp() );

        Interval new_range(0,0);
        vector<Interval>::iterator it = intervals.begin() + 1;
        while( it != intervals.end() ) {
            bool merged = merge_range( *(it-1), *it, new_range );
            if( merged ) {
                *it = new_range;
                it = intervals.erase( it - 1 );
            } // if 
            ++it;
        } // while 

        return intervals;
    }
};


int main( int argc, char **argv )
{
    Solution sln;

    vector<Interval> intervals;
    Interval tmp(0,0);

    while( cin >> tmp )
        intervals.push_back( tmp );

    cout << intervals.size() << endl;

    intervals = sln.merge( intervals );

    copy( intervals.begin(), intervals.end(), ostream_iterator<Interval>(cout, " ") );
    cout << endl;

    // Interval i1(1,3), i2(2,6), i3;
    // merge_range(i1,i2,i3);
    // cout << i3 << endl;

    return 0;
}
