#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <LOG.h>

 
int main()
{
    std::chrono::duration< int, std::ratio<1, 30> > hz(1);
    int count = 0;

    while( true ) {
        DBG( "working %d", ++count );
        std::this_thread::sleep_for(hz);
    }

    return 0;
}











