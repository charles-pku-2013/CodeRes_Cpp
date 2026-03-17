/*
c++ -o /tmp/test thread_pool_timeout.cpp -std=c++17 -pthread -lfmt -g
 */
#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

// 引入 fmt 库
#include <fmt/core.h>
#include <fmt/std.h>

class ThreadPool {
public:
    explicit ThreadPool(size_t threads);

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>;

    // 清理排队时间超过 timeout_ms 的任务
    size_t purge_timeout_tasks(std::chrono::milliseconds timeout_ms);

    ~ThreadPool();

private:
    // 内部任务结构体：包含可执行函数和入队时间
    struct TaskWrapper {
        std::function<void()>                 func;
        std::chrono::steady_clock::time_point submit_time;
    };

    std::vector<std::thread> workers;
    // 使用 std::deque 替代 std::queue，以便进行遍历和中间删除
    std::deque<TaskWrapper> tasks;

    std::mutex              queue_mutex;
    std::condition_variable condition;
    bool                    stop;
};

// --- 构造函数 ---
inline ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            for (;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] {
                        return this->stop || !this->tasks.empty();
                    });

                    if (this->stop && this->tasks.empty()) {
                        return;
                    }

                    // 获取任务闭包，并将其从队列前端弹出
                    task = std::move(this->tasks.front().func);
                    this->tasks.pop_front();
                }
                // 执行任务
                task();
            }
        });
    }
}

// --- 入队方法 ---
template <class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
    using return_type = std::invoke_result_t<F, Args...>;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        // 将任务和当前时间戳一起存入双端队列
        tasks.push_back({[task]() {
                             (*task)();
                         },
                         std::chrono::steady_clock::now()});
    }
    condition.notify_one();
    return res;
}

// --- 强制清理超时任务 ---
inline size_t ThreadPool::purge_timeout_tasks(std::chrono::milliseconds timeout_ms) {
    std::unique_lock<std::mutex> lock(queue_mutex);
    if (tasks.empty()) {
        return 0;
    }

    auto   now = std::chrono::steady_clock::now();
    size_t removed_count = 0;

    // 遍历双端队列
    for (auto it = tasks.begin(); it != tasks.end();) {
        // 如果当前时间 - 提交时间 > 设定的超时阈值
        if (now - it->submit_time > timeout_ms) {
            it = tasks.erase(it);  // 删除该任务，erase 返回指向下一个元素的迭代器
            removed_count++;
        } else {
            ++it;
        }
    }
    return removed_count;
}

// --- 析构函数 ---
inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

// ==========================================
// 测试用例
// ==========================================
int main() {
    // 创建一个只有 2 个工作线程的线程池
    ThreadPool                    pool(2);
    std::vector<std::future<int>> results;

    fmt::print(">>> 线程池已启动，开始提交 10 个耗时任务...\n");

    // 瞬间塞入 10 个任务。因为只有 2 个线程，前 2 个任务会立刻执行，后 8 个会在队列中等待
    for (int i = 0; i < 10; ++i) {
        results.emplace_back(pool.enqueue([i] {
            fmt::print("  -> 任务 {} 开始执行 (线程: {})\n", i, std::this_thread::get_id());
            // 模拟耗时 2 秒的操作
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return i * 10;
        }));
    }

    // 主线程等待 1.5 秒。
    // 此时任务 0 和 1 还在执行中（耗时2秒）。
    // 任务 2 到 9 已经在队列里苦等了 1.5 秒了。
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    // 强制清理排队时间超过 1 秒的任务。
    // 预期：任务 2 到 9 会被全部清理掉。
    size_t removed = pool.purge_timeout_tasks(std::chrono::milliseconds(1000));
    fmt::print("\n>>> 主线程发起清理：移除了 {} 个超时的排队任务！ <<<\n\n", removed);

    // 等待所有任务并获取结果
    for (size_t i = 0; i < results.size(); ++i) {
        try {
            // 注意：被清理的任务在这里调用 get() 会抛出异常
            int res = results[i].get();
            fmt::print("任务 {} 成功完成，结果: {}\n", i, res);
        } catch (const std::future_error& e) {
            // 捕获 Broken promise 异常，证明任务被成功丢弃
            fmt::print("任务 {} 异常终止 (原因: 被从队列清理, {})\n", i, e.what());
        } catch (const std::exception& e) {
            fmt::print("任务 {} 发生其他异常: {}\n", i, e.what());
        }
    }

    return 0;
}
