#include <iostream>
#include <thread>
 
int main() 
{
    // 😟😟只能检测到一个物理CPU
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

    return 0;
}
