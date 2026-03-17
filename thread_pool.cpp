/*
c++ -o /tmp/test thread_pool.cpp -std=c++17 -pthread -lfmt -g
 */
#include <fmt/core.h>
#include <fmt/std.h>
#include <pthread.h>

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
public:
    // 构造函数，启动指定数量的工作线程
    explicit ThreadPool(size_t threads);

    // 添加新任务到任务队列
    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>;

    // 析构函数，等待所有线程结束
    ~ThreadPool();

    // 危险操作：系统级暴力杀死所有线程（仅限 macOS/Linux 等 POSIX 系统）
    void force_kill();

private:
    // 需要追踪的线程
    std::vector<std::thread> workers;
    // 任务队列
    std::queue<std::function<void()>> tasks;

    // 同步相关的原语
    std::mutex              queue_mutex;
    std::condition_variable condition;
    bool                    stop;
};

// 构造函数：初始化工作线程
inline ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            for (;;) {
                std::function<void()> task;

                {
                    // 加锁获取任务
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] {
                        return this->stop || !this->tasks.empty();
                    });

                    // 如果线程池停止且没有任务了，就退出线程
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }

                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                // 执行任务
                task();
            }
        });
    }
}

// 核心：添加任务的方法
template <class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
    using return_type = std::invoke_result_t<F, Args...>;

    // 将任务打包成 packaged_task，以便通过 future 获取返回值
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // 如果线程池已经停止，不允许添加新任务
        if (stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        // 将任务包装成无参无返回值的形式，放入队列
        tasks.emplace([task]() {
            (*task)();
        });
    }
    // 唤醒一个等待中的工作线程
    condition.notify_one();
    return res;
}

// 析构函数：优雅地关闭线程池
inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    // 唤醒所有线程，让它们检查 stop 标志
    condition.notify_all();
    for (std::thread& worker : workers) {
        worker.join();  // 等待所有线程执行完毕
    }
}

inline void ThreadPool::force_kill() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            // 获取底层的 pthread_t 句柄
            pthread_t handle = worker.native_handle();

            // 发送取消请求，强制终止该线程
            pthread_cancel(handle);

            // 即使强杀了，也需要 detach 或 join 回收线程对象本身的资源
            worker.detach();
        }
    }
    workers.clear();
}

int main() {
    // 创建一个包含 4 个工作线程的线程池
    ThreadPool pool(4);

    // 保存任务结果的 future 集合
    std::vector<std::future<int>> results;

    for (int i = 0; i < 8; ++i) {
        // 将任务添加到线程池
        results.emplace_back(pool.enqueue([i] {
            // 使用 fmt::print 直接输出，天然防交错
            fmt::print("任务 {} 正在由线程 {} 执行\n", i, std::this_thread::get_id());

            // 模拟耗时操作
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return i * i;  // 返回计算结果
        }));
    }

    // 等待所有任务完成并获取结果
    for (auto&& result : results) {
        // 获取结果并使用 fmt::print 输出
        fmt::print("任务结果: {}\n", result.get());
    }

    return 0;
}
