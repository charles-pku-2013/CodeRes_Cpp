#ifndef _LOCK_FREE_QUEUE_
#define _LOCK_FREE_QUEUE_

#include <boost/lockfree/queue.hpp>
#include <climits>
#include <thread>
#include <chrono>

/*
 * push 操作用 bounded_push, 受指定初始size限制
 */

template <typename T>
class LockFreeQueue {
public:
    explicit LockFreeQueue(std::size_t sz)
                : m_Queue(sz) {}

    bool push(const T& value, int timeout = INT_MAX)
    {
        auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);

        while (!m_Queue.bounded_push(value)) {
            std::this_thread::yield();
            auto now = std::chrono::steady_clock::now();
            if (now >= deadline)
                return false;
        } // while

        return true;
    }

    bool pop(T& value, int timeout = INT_MAX)
    {
        auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
        
        while (!m_Queue.pop(value)) {
            std::this_thread::yield();
            auto now = std::chrono::steady_clock::now();
            if (now >= deadline)
                return false;
        } // while

        return true;
    }

    bool is_lock_free() const { return m_Queue.is_lock_free(); }

protected:
    boost::lockfree::queue<T>   m_Queue;
};

#endif

