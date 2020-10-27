#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <unistd.h>

#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
#define SLEEP_MICROSECONDS(x) std::this_thread::sleep_for(std::chrono::microseconds(x))

using namespace std;

// full cpu usage
static
void thread_routine1()
{
    static uint32_t i = 2;
    while (true)
        ++i;    // ❗❗也是全速运行，与具体执行什么运算无关
        // i *= i;
}

// SLEEP_MICROSECONDS(1):half; 100:70% 10:200%
static
void thread_routine2()
{
    static uint32_t i = 2;
    while (true) {
        i *= i;
        SLEEP_MICROSECONDS(10);
    } // while
}
 
int main() 
{
    // 😟😟只能检测到一个物理CPU
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

    cout << sysconf(_SC_NPROCESSORS_ONLN) << endl;

    // boost::thread_group thrgroup;
    // for( int i = 1; i <= n; ++i )
        // thrgroup.create_thread(thread_routine1);
    // thrgroup.join_all();

    return 0;
}
