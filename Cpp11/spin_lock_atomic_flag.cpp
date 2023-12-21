// https://en.cppreference.com/w/cpp/atomic/atomic_flag
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>
 
std::atomic_flag lock = ATOMIC_FLAG_INIT;
 
void f(int n)
{
    for (int cnt = 0; cnt < 40; ++cnt) {
        while (lock.test_and_set(std::memory_order_acquire)) {  // acquire lock
        // Since C++20, it is possible to update atomic_flag's
        // value only when there is a chance to acquire the lock.
        // See also: https://stackoverflow.com/questions/62318642
        #if defined(__cpp_lib_atomic_flag_test)
            while (lock.test(std::memory_order_relaxed))        // test lock
        #endif
                ; // spin
        }
        static int out{};
        std::cout << n << ((++out % 40) == 0 ? '\n' : ' ');
        lock.clear(std::memory_order_release);                  // release lock
    }
}
 
int main()
{
    std::vector<std::thread> v;
    for (int n = 0; n < 10; ++n) {
        v.emplace_back(f, n);
    }
    for (auto& t : v) {
        t.join();
    }

    return 0;
}

class SpinLock {
 public:
    class Holder;

 private:
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void unlock() {
        locked.clear(std::memory_order_release);
    }

    std::atomic_flag locked = ATOMIC_FLAG_INIT;
};

class SpinLock::Holder {
 public:
    explicit Holder(SpinLock *lock): lock(lock) {
        lock->lock();
    }

    ~Holder() {
        lock->unlock();
    }

    void release() {
        // sometimes you may want to manually unlock
        lock->unlock();
    }

 private:
    SpinLock* lock;
};
