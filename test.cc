#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <optional>

template <typename T>
class ThreadSafeBoundedQueue {
public:
    explicit ThreadSafeBoundedQueue(size_t capacity) : capacity_(capacity) {}

    // Timed push operation
    // Returns true if successful, false if timeout occurred or queue is closed
    template<typename Rep, typename Period>
    bool push_with_timeout(T item, const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Wait until the queue is not full or the timeout expires
        if (!not_full_cv_.wait_for(lock, timeout, [this] { return queue_.size() < capacity_; })) {
            // Timeout occurred
            return false;
        }

        // If we woke up because the condition is met (not full)
        queue_.push(std::move(item));
        lock.unlock();
        not_empty_cv_.notify_one(); // Notify a waiting consumer
        return true;
    }

    // Timed pop operation
    // Returns std::optional<T> with the value if successful, std::nullopt if timeout occurred or queue is closed
    template<typename Rep, typename Period>
    std::optional<T> pop_with_timeout(const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);

        // Wait until the queue is not empty or the timeout expires
        if (!not_empty_cv_.wait_for(lock, timeout, [this] { return !queue_.empty(); })) {
            // Timeout occurred
            return std::nullopt;
        }

        // If we woke up because the condition is met (not empty)
        T item = std::move(queue_.front());
        queue_.pop();
        lock.unlock();
        not_full_cv_.notify_one(); // Notify a waiting producer
        return std::make_optional(std::move(item));
    }

    // Standard blocking push (no timeout)
    void push(T item) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_cv_.wait(lock, [this] { return queue_.size() < capacity_; });
        queue_.push(std::move(item));
        lock.unlock();
        not_empty_cv_.notify_one();
    }

    // Standard blocking pop (no timeout)
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_cv_.wait(lock, [this] { return !queue_.empty(); });
        T item = std::move(queue_.front());
        queue_.pop();
        lock.unlock();
        not_full_cv_.notify_one();
        return item;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_cv_; // Used by consumers to wait for items
    std::condition_variable not_full_cv_;  // Used by producers to wait for space
    const size_t capacity_;
};

int main() {
    ThreadSafeBoundedQueue<int> q(5); // Queue with capacity of 5

    // Example producer thread
    std::thread producer([&]() {
        for (int i = 0; i < 10; ++i) {
            if (q.push_with_timeout(i, std::chrono::milliseconds(500))) {
                std::cout << "Pushed: " << i << std::endl;
            } else {
                std::cout << "Push timeout for: " << i << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // Example consumer thread
    std::thread consumer([&]() {
        for (int i = 0; i < 10; ++i) {
            if (auto item = q.pop_with_timeout(std::chrono::milliseconds(300))) {
                std::cout << "Popped: " << *item << std::endl;
            } else {
                std::cout << "Pop timeout" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
    });

    producer.join();
    consumer.join();

    return 0;
}
