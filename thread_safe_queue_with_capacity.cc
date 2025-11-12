#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <chrono>

template <typename T>
class LimitedCapacityThreadSafeQueue {
private:
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cv_not_empty;
    std::condition_variable cv_not_full;
    const size_t capacity;

public:
    explicit LimitedCapacityThreadSafeQueue(size_t capacity) : capacity(capacity) {}

    void push(const T& value) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_not_full.wait(lock, [this] { return q.size() < capacity; }); // Wait if queue is full
        q.push(value);
        cv_not_empty.notify_one(); // Notify waiting consumers
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv_not_empty.wait(lock, [this] { return !q.empty(); }); // Wait if queue is empty
        T value = q.front();
        q.pop();
        cv_not_full.notify_one(); // Notify waiting producers
        return value;
    }

    bool empty() const {
        std::unique_lock<std::mutex> lock(mtx);
        return q.empty();
    }

    size_t size() const {
        std::unique_lock<std::mutex> lock(mtx);
        return q.size();
    }
};

// Example usage:
void producer(LimitedCapacityThreadSafeQueue<int>& queue, int id) {
    for (int i = 0; i < 10; ++i) {
        int value = id * 100 + i;
        queue.push(value);
        std::cout << "Producer " << id << " pushed: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void consumer(LimitedCapacityThreadSafeQueue<int>& queue, int id) {
    for (int i = 0; i < 10; ++i) {
        int value = queue.pop();
        std::cout << "Consumer " << id << " popped: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    LimitedCapacityThreadSafeQueue<int> queue(5); // Queue with capacity of 5

    std::thread p1(producer, std::ref(queue), 1);
    std::thread p2(producer, std::ref(queue), 2);
    std::thread c1(consumer, std::ref(queue), 1);
    std::thread c2(consumer, std::ref(queue), 2);

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    return 0;
}
