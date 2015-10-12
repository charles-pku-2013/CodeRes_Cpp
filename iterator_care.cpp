// 使用iterator期间不可以更改container，不可以添加，删除数据

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

int main()
{
    using namespace std;
    
    vector<int> vec;
    vec.push_back(1000);
    vector<int>::iterator it = vec.begin();
    printf( "%lx\n", &vec[0] );
    printf( "%d\n", *it );
    
    for( int i = 1; i <= 500; ++i )
        vec.push_back(i);
    
    printf( "%lx\n", &vec[0] );
    printf( "%d\n", *it );
    
    
    return 0;
}















