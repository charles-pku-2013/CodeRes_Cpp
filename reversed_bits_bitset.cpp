#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
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
#include <cmath>
#include <cassert>
#include <exception>
#include <climits>
#include <bitset>
// #include "LOG.h"

// https://leetcode.com/problems/reverse-bits/

using namespace std; 

typedef unsigned int        uint32_t;

class Solution {
public:
    uint32_t reverseBits(uint32_t n) 
    {
        bitset<32> bits(n);
        string s = bits.to_string();

        reverse( s.begin(), s.end() );

        bitset<32> reversedBits( s );

        return reversedBits.to_ulong();
    }
};


int main( int argc, char **argv )
{
    uint32_t i = 43261596;

    Solution sln;
    cout << sln.reverseBits(i) << endl;

    return 0;
}
