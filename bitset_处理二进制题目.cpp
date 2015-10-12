#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <vector>
#include <iterator>
#include <bitset>

using namespace std;


class Solution {
public:
    static bool is_one( char ch )
    { return (ch == '1'); }

    /**
     * @param num: an integer
     * @return: an integer, the number of ones in num
     */
    int countOnes(int num) 
    {
        // 可以像这样用整数构造
        bitset<32> bits( (unsigned int)num );
        string s = bits.to_string();
        int count = count_if( s.begin(), s.end(), is_one );

        return count;
    }
};

int main( int argc, char **argv )
{
    Solution s;
    cout << s.countOnes( 1023 ) << endl;

    return 0;
}










