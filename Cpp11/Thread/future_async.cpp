#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;


int accum(int* beg, int* end, int init)
// compute the sum of [beg:end) starting with the initial value init
{   
    thread::id this_id = this_thread::get_id();
    cout << "In thread " << this_id << endl;
    
    this_thread::sleep_for( chrono::seconds(1) );
    
    return accumulate( beg, end, init );
}

//!! 由async决定线程数
// spawn many tasks if v is large enough
int comp4( vector<int>& v )
{
    if( v.size() < 10000 ) 
        return accumulate(v.begin(), v.end(), 0);
    
    auto v0 = &v[0];
    auto sz = v.size();
    auto f0 = async(accum, v0 ,v0 + sz/4, 0); // first quarter
    auto f1 = async(accum, v0 + sz/4, v0+sz/2, 0); // second quarter
    auto f2 = async(accum, v0+sz/2 ,v0+sz*3/4, 0); // third quarter
    auto f3 = async(accum, v0+sz*3/4, v0+sz, 0); // four th quar ter
    return f0.get() + f1.get() + f2.get() + f3.get(); // collect and combine the results
}


int main()
{
    vector<int> v;
    for( int i = 1; i <= 1000000; ++i )  
        v.push_back(i);
    int sum = comp4(v);
    cout << "sum = " << sum << endl;
    return 0;
}














