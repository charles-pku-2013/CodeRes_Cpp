#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>

#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

int main()
{
    printf("\rHello, world!");
    fflush(stdout);

    SLEEP_SECONDS(1);

    printf("\rCharles");
    fflush(stdout);
    putchar('\n');

    return 0;
}

