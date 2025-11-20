#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

std::timed_mutex g_timed_mutex; // Global timed_mutex

void worker_function() {
    std::cout << "Worker thread: Attempting to acquire lock..." << std::endl;

    // Try to acquire the lock for 1 second
    std::unique_lock<std::timed_mutex> lock(g_timed_mutex, std::defer_lock);
    if (lock.try_lock_for(std::chrono::seconds(1))) {
        std::cout << "Worker thread: Lock acquired!" << std::endl;
        // Simulate some work while holding the lock
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Worker thread: Releasing lock." << std::endl;
    } else {
        std::cout << "Worker thread: Failed to acquire lock within 1 second timeout." << std::endl;
    }
}

int main() {
    // Lock the mutex in the main thread to demonstrate the timeout
    std::unique_lock<std::timed_mutex> main_lock(g_timed_mutex);
    std::cout << "Main thread: Acquired lock." << std::endl;

    std::thread worker(worker_function);

    // Keep the main thread's lock for a short duration
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Release the main thread's lock
    main_lock.unlock();
    std::cout << "Main thread: Released lock." << std::endl;

    worker.join();

    return 0;
}
