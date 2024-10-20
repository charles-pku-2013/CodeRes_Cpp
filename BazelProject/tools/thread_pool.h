#pragma once
#include <cassert>
#include <cstddef>
#include <functional>
#include <queue>
#include <thread>  // NOLINT(build/c++11)
#include <vector>

#include "absl/base/thread_annotations.h"
#include "absl/synchronization/mutex.h"

namespace tools {

template<typename Callable>
class ThreadPool {
 public:
    explicit ThreadPool(int num_threads) {
        for (int i = 0; i < num_threads; ++i) {
            threads_.push_back(std::thread(&ThreadPool::WorkLoop, this));
        }
    }

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;

    ~ThreadPool() {
        {
            absl::MutexLock l(&mu_);
            for (size_t i = 0; i < threads_.size(); i++) {
                queue_.push(nullptr);  // Shutdown signal.
            }
        }
        for (auto &t : threads_) {
            if (t.joinable()) { t.join(); }
        }
    }

    // Schedule a function to be run on a ThreadPool thread immediately.
    // void Schedule(std::function<void()> func) {
        // assert(func != nullptr);
        // absl::MutexLock l(&mu_);
        // queue_.push(std::move(func));
    // }
    void Schedule(const std::function<void()> &func) {
        assert(func != nullptr);
        absl::MutexLock l(&mu_);
        queue_.push(func);
    }

    void Cancel() {
        for (auto &t : threads_) {
            if (t.joinable()) {
                auto tid = t.native_handle();
                if (tid) { ::pthread_cancel(tid); }
            }
        }
    }

    std::size_t NumWorkers() const
    { return threads_.size(); }

 private:
    bool WorkAvailable() const ABSL_EXCLUSIVE_LOCKS_REQUIRED(mu_) {
        return !queue_.empty();
    }

    void WorkLoop() {
        while (true) {
            std::function<void()> func;
            {
                absl::MutexLock l(&mu_);
                mu_.Await(absl::Condition(this, &ThreadPool::WorkAvailable));
                func = std::move(queue_.front());
                queue_.pop();
            }
            if (func == nullptr) {  // Shutdown signal.
                break;
            }
            func();
        }
    }

    absl::Mutex mu_;
    std::queue<std::function<void()>> queue_ ABSL_GUARDED_BY(mu_);
    std::vector<std::thread> threads_;
};

}  // namespace tools
