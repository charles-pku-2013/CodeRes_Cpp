#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdio>

using namespace std;

template < typename T >
void print( const T &c )
{
    for( typename T::const_iterator it = c.begin(); it != c.end(); ++it )
        cout << *it << " ";
    cout << endl;
}

int main()
{
    int a[] = {1,3,5,5,5,7,9};
    int b[] = {2,3,5,5,6,9,10};
    
    vector<int> vec;
    
    //!! 可能保留重复元素，当两个源表都有相同重复元素时候， 总之，若要去除重复，需要unique
//    set_intersection(a, a + sizeof(a)/sizeof(*a), b, b + sizeof(b)/sizeof(*b), back_inserter(vec));
    //!! 保留重复元素
//    set_union(a, a + sizeof(a)/sizeof(*a), b, b + sizeof(b)/sizeof(*b), back_inserter(vec));
    
    set_difference(a, a + sizeof(a)/sizeof(*a), b, b + sizeof(b)/sizeof(*b), back_inserter(vec));
    print(vec);
    
    return 0;
}











