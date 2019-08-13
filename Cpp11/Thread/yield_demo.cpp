#include <iostream>
#include <chrono>
#include <thread>

#define SLEEP_MICROSECONDS(x) std::this_thread::sleep_for(std::chrono::microseconds(x))

void test1()
{
    while (true)
        std::this_thread::yield();
        // SLEEP_MICROSECONDS(1);
}

// "busy sleep" while suggesting that other threads run
// for a small amount of time
void little_sleep(std::chrono::microseconds us)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + us;
    do {
        std::this_thread::yield();
    } while (std::chrono::high_resolution_clock::now() < end);
}

int main()
{
    test1();
    return 0;

    auto start = std::chrono::high_resolution_clock::now();

    little_sleep(std::chrono::microseconds(100));

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    std::cout << "waited for "
              << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
              << " microseconds\n";

    return 0;
}
