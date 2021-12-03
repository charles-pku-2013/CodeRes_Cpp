#pragma once
#include <deque>

#include "absl/base/thread_annotations.h"
#include "absl/synchronization/mutex.h"

namespace tools {

template<typename T>
class ThreadSafeQueue {
 public:
    bool available() const ABSL_EXCLUSIVE_LOCKS_REQUIRED(mu_)
    { return !queue_.empty(); }

    std::size_t size() const ABSL_EXCLUSIVE_LOCKS_REQUIRED(mu_)
    { return !queue_.size(); }

    void push(const T& item) {
        absl::MutexLock l(&mu_);
        queue_.push_back(item);
    }

    T pop() {
        absl::MutexLock l(&mu_);
        mu_.Await(absl::Condition(this, &ThreadSafeQueue<T>::available));
        T item = std::move(queue_.front());
        queue_.pop_front();
        return item;
    }

 private:
    absl::Mutex mu_;
    std::deque<T> queue_ ABSL_GUARDED_BY(mu_);
};

}  // namespace tools
