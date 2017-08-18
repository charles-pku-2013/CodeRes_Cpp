#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

 
int main() 
{
    // 😟😟只能检测到一个物理CPU
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

    cout << sysconf(_SC_NPROCESSORS_ONLN) << endl;

    return 0;
}
