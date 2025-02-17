#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>

using namespace std;

/*
 * std::chrono::nanoseconds
 * std::chrono::microseconds
 * std::chrono::milliseconds
 * std::chrono::seconds
 * std::chrono::minutes
 * std::chrono::hours
 */

static
void test1()
{
    volatile int sink;

    for (auto size = 1ull; size < 1000000000ull; size *= 100) {
        // record start time
        auto start = std::chrono::high_resolution_clock::now();
        // do some work
        std::vector<int> v(size, 42);
        sink = std::accumulate(v.begin(), v.end(), 0u); // make sure it's a side effect
        // record end time
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::cout << "Time to fill and iterate a vector of "
                  << size << " ints : " << diff.count() << " s\n";
    } // for
}

static
void test2()
{
    // high_resolution_clock may be an alias of std::chrono::system_clock or std::chrono::steady_clock, or a third, independent clock.
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    cout << typeid(start).name() << endl;  // NSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEE

    int sum = 0;
    for (int i = 1; i <= 10000000; ++i)
        sum += i;

    auto end = std::chrono::high_resolution_clock::now();
    cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;
}

int main()
{
    test2();

    return 0;
}

