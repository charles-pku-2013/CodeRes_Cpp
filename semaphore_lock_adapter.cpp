#include <iostream>
#include <semaphore>
#include <mutex>
#include <thread>

// 1. 信号量适配器：将 lock()/unlock() 映射到 acquire()/release()
template <typename Semaphore>
struct semaphore_lock_adapter {
    Semaphore& sem;

    void lock() { sem.acquire(); }
    void unlock() { sem.release(); }
    bool try_lock() { return sem.try_acquire(); }
};

std::binary_semaphore sem{1}; // 初始许可数为 1

void critical_section_work() {
    semaphore_lock_adapter adapter{sem};

    // 2. 模拟在某些复杂逻辑中，手动调用了 acquire() 获取信号量
    std::cout << "[线程 " << std::this_thread::get_id() << "] 显式调用 sem.acquire()...\n";
    sem.acquire();

    // 3. 传入 std::adopt_lock：告知 lock_guard 锁已经被当前线程持有，
    //    构造函数内不会再次调用 adapter.lock()，但析构时依然会自动调用 adapter.unlock()（即 sem.release()）
    {
        std::lock_guard<semaphore_lock_adapter<std::binary_semaphore>> lock(adapter, std::adopt_lock);

        std::cout << "[线程 " << std::this_thread::get_id() << "] 进入 RAII 保护的临界区...\n";
        // 如果这里抛出异常或中途 return，lock_guard 也会安全地释放信号量
    }
    // 4. 离开作用域，lock_guard 析构，自动触发 adapter.unlock() -> sem.release()

    std::cout << "[线程 " << std::this_thread::get_id() << "] 已自动释放信号量！\n";
}

int main() {
    std::thread t1(critical_section_work);
    std::thread t2(critical_section_work);

    t1.join();
    t2.join();

    return 0;
}
