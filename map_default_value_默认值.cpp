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

struct Value {
    Value() : val(0)
    { printf("Value default constructor: val = %d, addr = %lx\n", val, (size_t)this); }

    Value& operator++()
    {
        ++val;
        printf("++Value@%lx, val = %d\n", (size_t)this, val);
        return *this;
    }

    int val;
};

int main( int argc, char **argv )
{
    map< int, Value > dict;
    Value v = dict[1];
    ++dict[2];

    return 0;
}
