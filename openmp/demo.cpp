/*
 * c++ -o /tmp/test test.cpp -fopenmp -std=c++11 -pthread -g
 */
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
// #include <omp.h>

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


int main()
{
    const int n = 5;

    cout << "Start parallel for." << endl;

#pragma omp parallel for
    for (int i = 1; i <= n; ++i)
        func(i);

    cout << "parallel for end." << endl;

    return 0;
}
