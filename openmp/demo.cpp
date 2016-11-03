/*
 * c++ -o /tmp/test demo.cpp -lglog -fopenmp -std=c++11 -pthread -g
 */
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <glog/logging.h>


#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

using namespace std;

void func( int i )
{
#pragma omp critical
    cout << "Thread " << THIS_THREAD_ID << " will sleep " << i << " seconds" << endl;

    SLEEP_SECONDS(i);
}

void test1()
{
    const int n = 7;

    cout << "Start parallel for." << endl;

#pragma omp parallel for
    for (int i = 1; i <= n; ++i)
        func(i);

    //!! 等所有的parallel thread完成后才执行，same as join_all()
    cout << "parallel for end." << endl;
}

void test_sum_vec()
{
    int sum = 0;
    vector<int> vec{1,2,3,4,5,6,7,8,9,10};

#pragma omp parallel for reduction (+:sum)
    for (size_t i = 0; i < vec.size(); ++i) {
        LOG(INFO) << "processing " << i;
        sum += vec[i];
    } // for

    cout << "sum = " << sum << endl;
}

int main()
{
    // test1();
    test_sum_vec();

    return 0;
}
