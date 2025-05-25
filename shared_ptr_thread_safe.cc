/*
 * All member functions (including copy constructor and copy assignment) can be called by multiple
 * threads on different instances of shared_ptr without additional synchronization even if these
 * instances are copies and share ownership of the same object.
 *
 * If multiple threads of execution * access the same instance of shared_ptr without synchronization
 * and any of those accesses uses a non-const member function of shared_ptr then a data race will
 * occur; the shared_ptr overloads of atomic functions can be used to prevent the data race.
 *
 * 不同的shared_ptr实例，即使指向同一对象，那么所有的成员函数都是线程安全的
 * 同一shared_ptr实例，只有const成员函数是线程安全的 non-const: operator= reset swap
 */

// template< class T >
// struct std::atomic<std::shared_ptr<T>>;
// (since C++20)

#include <memory>
#include <mutex>

std::shared_ptr<int> ptr = std::make_shared<int>(42);
std::mutex           mtx;

// 线程安全操作：不同线程操作独立的 shared_ptr 实例
void safe_op() {
    auto local_ptr = ptr;  // 原子递增引用计数
    // 使用 local_ptr...
}

// NOTE 读写需要同步
// 需同步的操作：多线程修改同一 shared_ptr 实例
void unsafe_op() {
    std::lock_guard<std::mutex> lock(mtx);
    ptr.reset(new int(100));  // 加锁保证安全
}
