#include <boost/circular_buffer.hpp>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <optional>
#include <chrono>

template <typename T>
class SharedQueue {
public:
    using Queue = boost::circular_buffer<T>;

    explicit SharedQueue(std::size_t capacity) : queue_(capacity) {}

    void push(T item) {
        std::unique_lock lk(mutex_);

        cv_not_full_.wait(lk, [this]() -> bool {
            return !queue_.full();
        });

        queue_.push_back(std::move(item));
        lk.unlock();
        cv_not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock lk(mutex_);

        cv_not_empty_.wait(lk, [this]() -> bool {
            return !queue_.empty();
        });

        T item = std::move(queue_.front());
        queue_.pop_front();

        lk.unlock();
        cv_not_full_.notify_one();  // Notify waiting producers
        return item;
    }

    template <typename Duration>
    bool timeout_push(T item, const Duration& timeout) {
        std::unique_lock lk(mutex_, std::defer_lock);
        if (!lk.try_lock_for(timeout)) {
            return false;
        }

        if (!cv_not_full_.wait_for(lk, timeout, [this]() -> bool {
                return !queue_.full();
            })) {
            return false;
        }

        queue_.push_back(std::move(item));
        lk.unlock();
        cv_not_empty_.notify_one();

        return true;
    }

    template <typename Duration>
    std::optional<T> timeout_pop(const Duration& timeout) {
        std::unique_lock lk(mutex_, std::defer_lock);
        if (!lk.try_lock_for(timeout)) {
            return std::nullopt;
        }

        if (!cv_not_empty_.wait_for(lk, timeout, [this]() -> bool {
                return !queue_.empty();
            })) {
            return std::nullopt;
        }

        T item = std::move(queue_.front());
        queue_.pop_front();

        lk.unlock();
        cv_not_full_.notify_one();  // Notify waiting producers

        return std::make_optional(std::move(item));
    }

    std::size_t size() const {
        std::unique_lock lk(mutex_);
        return queue_.size();
    }

    bool empty() const {
        std::unique_lock lk(mutex_);
        return queue_.empty();
    }

private:
    Queue                    queue_;
    mutable std::timed_mutex mutex_;

    // std::condition_variable in C++ is designed to work exclusively with std::unique_lock<std::mutex>. 
    // It cannot be directly associated with std::unique_lock<std::timed_mutex> or any other lock type.
    // std::condition_variable  cv_not_empty_, cv_not_full_;

    std::condition_variable_any  cv_not_empty_, cv_not_full_;
};

int main() {
    SharedQueue<int> que(2);

    que.push(10);
    auto item = que.timeout_pop(std::chrono::seconds(1));
    if (!item) {
        std::cout << "timeout" << std::endl;
    } else {
        std::cout << *item << std::endl;
    }
    return 0;

    std::thread t1([&] {
        que.push(1);
        std::cout << "Producer pushing 1" << std::endl;
        que.push(2);
        std::cout << "Producer pushing 2" << std::endl;
        que.push(3);
        std::cout << "Producer pushing 3" << std::endl;
        que.push(0);
        std::cout << "Producer pushing 0" << std::endl;
    });

    std::thread t2([&] {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        while (true) {
            int v = que.pop();
            std::cout << "Consumer get " << v << std::endl;
            if (!v) {
                break;
            }
        }
    });

    t1.join();
    t2.join();

    return 0;
}
