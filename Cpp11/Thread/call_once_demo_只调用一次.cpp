#include <iostream>
#include <thread>
// #include <mutex>
 
std::once_flag flag;
 
void do_once()
{
    std::call_once(flag, [](){ std::cout << "Called once" << std::endl; });
}
 
int main()
{
    do_once();
    do_once();
    do_once();
    do_once();
    do_once();
    exit(0);


    std::thread t1(do_once);
    std::thread t2(do_once);
    std::thread t3(do_once);
    std::thread t4(do_once);
 
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
