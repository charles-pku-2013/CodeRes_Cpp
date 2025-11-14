#include <boost/circular_buffer.hpp>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

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
        cv_not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock lk(mutex_);

        cv_not_empty_.wait(lk, [this]() -> bool {
            return !queue_.empty();
        });

        T item = std::move(queue_.front());
        queue_.pop_front();

        cv_not_full_.notify_one();  // Notify waiting producers
        return item;
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
    Queue                   queue_;
    mutable std::mutex      mutex_;
    std::condition_variable cv_not_empty_, cv_not_full_;
};

int main() {
    SharedQueue<int> que(2);

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
