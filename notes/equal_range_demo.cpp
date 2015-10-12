#include <cstdio>
#include <set>
#include <algorithm>

using namespace std;

typedef pair<set<int>::iterator, set<int>::iterator> EqualRangeRetType;

int main()
{
    set<int> s;
    EqualRangeRetType ret;

    for( int i = 2; i <= 10; i += 2 )
        s.insert(i);
    
    // 若查找元素存在，first = second >
    ret = equal_range(s.begin(), s.end(), 6);
    printf( "%d %d\n", *(ret.first), *(ret.second) );
    // the last element, first = second is end()
    ret = equal_range(s.begin(), s.end(), 10);
    printf( "%d %d\n", *(ret.first), *(ret.second) );
    printf( "%d\n", ret.second == s.end() );
    
    // 若查找元素存在，first == second all >
    ret = equal_range(s.begin(), s.end(), 5);
    printf( "%d %d\n", *(ret.first), *(ret.second) );
    // again
    ret = equal_range(s.begin(), s.end(), 1);
    printf( "%d %d\n", *(ret.first), *(ret.second) );
    // beyond the last, first == second == end()
    ret = equal_range(s.begin(), s.end(), 11);
    printf( "%d %d\n", ret.first == s.end(), ret.second == s.end() );
    
    return 0;
}


















