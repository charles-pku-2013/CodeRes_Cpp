#include "task_queue.h"

#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>
#include <glog/logging.h>

#include <string>

namespace newtranx {

TimeoutTaskItem::~TimeoutTaskItem() {
    stopTimer();
}

void TimeoutTaskItem::startTimer(IoService& io_service, int32_t timeout_in_ms) {
    if (timeout_in_ms > 0) {
        timer_.reset(new Timer(io_service));
        timer_->expires_from_now(boost::posix_time::milliseconds(timeout_in_ms));
        timer_->async_wait([this](boost::system::error_code ec) {
            if (!ec) {
                // 正常计时结束是0，cancel是125
                status_ = Status::TIMEOUT;
                try {
                    onTimeout();
                } catch (const std::exception& ex) {
                    LOG(ERROR) << fmt::format("Task {} timeout handler exception: {}",
                                              DebugString(), ex.what());
                }
                cv_.notify_all();
            }
        });
    }
}

void TimeoutTaskItem::stopTimer() {
    if (timer_) {
        timer_->cancel();
        timer_.reset();
    }
}

void TimeoutTaskItem::doJob() {
    try {
        jobRoutine();
    } catch (const std::exception& ex) {
        LOG(ERROR) << fmt::format("Task {} JobRoutine exception: {}", DebugString(), ex.what());
    }
    status_ = Status::DONE;
    cv_.notify_all();
}

// NOTE duration as argument
bool TimeoutTaskItem::wait(const std::chrono::milliseconds& duration) {
    std::unique_lock lk(mutex_);
    return cv_.wait_for(lk, duration, [this]() -> bool {
        return status() == Status::DONE || status() == Status::TIMEOUT;
    });
}

std::string TimeoutTaskItem::DebugString() const {
    return "TimeoutTaskItem";
}

TimeoutTaskQueue::TimeoutTaskQueue(std::size_t capacity, int32_t timeout_in_ms, int32_t n_workers,
                                   int32_t n_io_threads)
    : queue_(capacity), timeout_(timeout_in_ms), n_workers_(n_workers) {
    // assert(timeout_ > 0);
    assert(n_workers_ > 0);

    io_service_.reset(new IoService);
    io_service_work_.reset(new IoServiceWork(std::ref(*io_service_)));

    for (int32_t i = 0; i < n_io_threads; ++i) {
        io_service_workers_.emplace_back([this] { io_service_->run(); });
    }

    // remove timeout tasks
    cleaner_interval_ = 10 * timeout_;
    if (cleaner_interval_ > 0) {
        cleaner_ = std::thread([&] {
            while (running_) {
                std::this_thread::sleep_for(std::chrono::milliseconds(cleaner_interval_));
                std::unique_lock lk(mutex_);
                auto             new_end = std::remove_if(
                    queue_.begin(), queue_.end(), [](const TimeoutTaskItem::pointer& item) -> bool {
                        return item->status() == TimeoutTaskItem::Status::TIMEOUT;
                    });
                DLOG(INFO) << fmt::format("Cleanning up queue, removed {} timeout tasks.",
                                          std::distance(new_end, queue_.end()));
                queue_.erase(new_end, queue_.end());
            }  // while
        });
    }  // if
}

TimeoutTaskQueue::~TimeoutTaskQueue() {
    clear();
    io_service_work_.reset();
    io_service_->stop();

    for (auto& thr : io_service_workers_) {
        if (thr.joinable()) {
            thr.join();
        }
    }

    auto cleaner_id = cleaner_.native_handle();
    pthread_cancel(cleaner_id);

    if (cleaner_.joinable()) {
        cleaner_.join();
    }
}

bool TimeoutTaskQueue::push(TimeoutTaskItem::pointer item) {
    std::unique_lock lk(mutex_);

    if (queue_.full() || !running_) {
        return false;
    }

    // start timeout counter down before push into the queue
    item->startTimer(*io_service_, timeout_);
    queue_.push_back(std::move(item));
    lk.unlock();
    cv_.notify_one();

    return true;
}

TimeoutTaskItem::pointer TimeoutTaskQueue::pop() {
    while (running_) {
        std::unique_lock lk(mutex_);
        while (queue_.empty() && running_) {
            cv_.wait(lk);
        }

        while (!queue_.empty() && running_) {
            auto item = std::move(queue_.front());
            queue_.pop_front();
            if (item && item->status() != TimeoutTaskItem::Status::TIMEOUT) {
                // stop timer and reset status before being scheduled
                item->stopTimer();
                item->setStatus(TimeoutTaskItem::Status::RUNNING);
                return item;
            }
        }
    }

    return TimeoutTaskItem::pointer();
}

void TimeoutTaskQueue::start() {
    if (running_) {
        return;
    }

    running_ = true;
    workers_.clear();

    for (int32_t i = 0; i < n_workers_; ++i) {
        workers_.emplace_back([this] {
            LOG(INFO) << fmt::format("TaskQueue worker {} running...", std::this_thread::get_id());
            while (running_) {
                auto task = pop();
                if (!task || !running_) {
                    return;
                }
                if (task && task->status() == TimeoutTaskItem::Status::RUNNING) {
                    DLOG(INFO) << fmt::format("Worker {} running task {}",
                                              std::this_thread::get_id(), task->DebugString());
                    task->doJob();
                }
            }
        });
    }
}

void TimeoutTaskQueue::stop() {
    if (!running_) {
        return;
    }
    LOG(INFO) << "TimeoutTaskQueue terminating...";

    running_ = false;

    std::unique_lock lk(mutex_);
    queue_.clear();
    cv_.notify_all();
    lk.unlock();

    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    LOG(INFO) << "TimeoutTaskQueue terminated.";
}

void TimeoutTaskQueue::clear() {
    std::unique_lock lk(mutex_);
    queue_.clear();
}

std::size_t TimeoutTaskQueue::size() const {
    std::unique_lock lk(mutex_);
    return queue_.size();
}

std::string TimeoutTaskQueue::DebugString() const {
    std::unique_lock lk(mutex_);
    return fmt::format(
        "TimeoutTaskQueue: {{size:{}, capacity:{}, timeout:{}, n_workers:{}, cleaner_interval:{}}}",
        queue_.size(), queue_.capacity(), timeout_, n_workers_, cleaner_interval_);
}

}  // namespace newtranx
