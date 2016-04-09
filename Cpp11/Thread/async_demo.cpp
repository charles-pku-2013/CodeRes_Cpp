#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <glog/logging.h>
 
template <typename RAIter>
int parallel_sum(RAIter beg, RAIter end)
{
    auto len = end - beg;

    LOG(INFO) << "len = " << len;

    if(len < 1000)
        return std::accumulate(beg, end, 0);
 
    RAIter mid = beg + len/2;
    // handle's type is future
    auto handle = std::async(std::launch::async,
                              parallel_sum<RAIter>, mid, end);
    int sum = parallel_sum(beg, mid);
    return sum + handle.get();
}
 
int main( int argc, char **argv )
{
    google::InitGoogleLogging(argv[0]);

    std::vector<int> v(1000000, 1);
    std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';

    return 0;
}
