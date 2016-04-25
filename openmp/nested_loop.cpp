/*
 * c++ -o /tmp/test nested_loop.cpp -std=c++11 -fopenmp -O3 -g
 */
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <omp.h>

#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

using namespace std;


int main()
{
    vector<int> arr = {1,2,3,4,5};

    auto process = [](int i1, int i2) {
#pragma omp critical
        cout << "Thread " << THIS_THREAD_ID << " " 
                << i1 << " * " << i2 << " = " << i1 * i2 << endl;
        SLEEP_MILLISECONDS(300);
#pragma omp critical
        cout << "Thread " << THIS_THREAD_ID << " Done!" << endl;
    };

// #pragma omp parallel for collapse(2)
// #pragma omp parallel for schedule(dynamic,1) collapse(2)
    // Below works OK!
// #pragma omp parallel for schedule(dynamic,1)
    // for (size_t i = 0; i < arr.size()-1; ++i) {
        // for (size_t j = i+1; j < arr.size(); ++j) { 
            // process( arr[i], arr[j] );
        // } // for j
    // } // for i

// above not work properly
/*
 * Thread 140483588994816 1 * 2 = 2
 * Thread 140483597387520 1 * 3 = 3
 * Thread 140483605780224 1 * 4 = 4
 * Thread 140483624736704 1 * 5 = 5
 * Thread 140483624736704 2 * 2 = 4
 * Thread 140483605780224 2 * 3 = 6
 * Thread 140483588994816 2 * 4 = 8
 * Thread 140483597387520 2 * 5 = 10
 * Thread 140483588994816 3 * 2 = 6
 * Thread 140483597387520 3 * 3 = 9
 * Thread 140483624736704 3 * 4 = 12
 * Thread 140483605780224 3 * 5 = 15
 * Thread 140483605780224 4 * 2 = 8
 * Thread 140483597387520 4 * 3 = 12
 * Thread 140483588994816 4 * 4 = 16
 * Thread 140483624736704 4 * 5 = 20
 */

    // 仅对最内层循环并行化
    // 但最外层循环是串行的，看下面结果
    // for (size_t i = 0; i < arr.size()-1; ++i) {
// #pragma omp parallel for
        // for (size_t j = i+1; j < arr.size(); ++j) { 
            // process( arr[i], arr[j] );
        // } // for j
    // } // for i

/*
 * Thread 139652612265728 1 * 3 = 3
 * Thread 139652631222208 1 * 2 = 2
 * Thread 139652595480320 1 * 5 = 5
 * Thread 139652603873024 1 * 4 = 4
 * Thread 139652612265728 Done!
 * Thread 139652631222208 Done!
 * Thread 139652595480320 Done!
 * Thread 139652603873024 Done!
 * Thread 139652612265728 2 * 4 = 8
 * Thread 139652631222208 2 * 3 = 6
 * Thread 139652603873024 2 * 5 = 10
 * Thread 139652603873024 Done!
 * Thread 139652612265728 Done!
 * Thread 139652631222208 Done!
 * Thread 139652612265728 3 * 5 = 15
 * Thread 139652631222208 3 * 4 = 12
 * Thread 139652612265728 Done!
 * Thread 139652631222208 Done!
 * Thread 139652631222208 4 * 5 = 20
 * Thread 139652631222208 Done!
 */

    // 都加上 parallel, 可以做到完全并行
    // Below works OK!
#pragma omp parallel for
    for (size_t i = 0; i < arr.size()-1; ++i) {
#pragma omp parallel for
        for (size_t j = i+1; j < arr.size(); ++j) { 
            process( arr[i], arr[j] );
        } // for j
    } // for i

/*
 * error: work-sharing region may not be closely nested inside of work-sharing, 
 * critical, ordered, master or explicit task region
 *             #pragma omp for
 */
// #pragma omp parallel
    // {
// #pragma omp for
        // for (size_t i = 0; i < arr.size()-1; ++i) {
// #pragma omp for
            // for (size_t j = i+1; j < arr.size(); ++j) { 
                // process( arr[i], arr[j] );
            // } // for j
        // } // for i
    // } // omp parallel

    return 0;
}
