/*
c++ -o /tmp/test timeout_task_queue.cc -lfmt -lglog -lgflags -std=c++17 -g
 */
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <glog/logging.h>

#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

namespace common {

using Timer = boost::asio::deadline_timer;
using IoServiceWork = boost::asio::io_service::work;
using IoService = boost::asio::io_service;

/**
 * @brief 带超时计时的任务项
 *        若规定时间内得不到调度，则执行超时处理逻辑 onTimeout
 */
class TimeoutTaskItem {
 public:
    using pointer = std::shared_ptr<TimeoutTaskItem>;

    enum class Status : int32_t { WAITTING, RUNNING, DONE, TIMEOUT };

 public:
    TimeoutTaskItem() = default;

    virtual ~TimeoutTaskItem() {
        stopTimer();
    }

    /**
     * @brief 开始超时计时
     *
     * @param io_service
     * @param timeout_in_ms  超时时间，单位毫秒
     */
    void startTimer(IoService& io_service, int32_t timeout_in_ms) {
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
                        fprintf(stderr, "Task %s timeout handler exception: %s\n", DebugString().c_str(), ex.what());
                    }
                    cv_.notify_all();
                }
            });
        }
    }

    /**
     * @brief 取消超时计时
     */
    void stopTimer() {
        if (timer_) {
            timer_->cancel();
            timer_.reset();
        }
    }

    /**
     * @brief timeout handler, implemented by subclasses
     */
    virtual void onTimeout() = 0;

    /**
     * @brief task 主任务逻辑
     */
    virtual void jobRoutine() = 0;

    virtual void doJob() {
        try {
            jobRoutine();
        } catch (const std::exception& ex) {
            fprintf(stderr, "Task %s JobRoutine exception: %s\n", DebugString().c_str(), ex.what());
        }
        status_ = Status::DONE;
        cv_.notify_all();
    }

    bool wait(const decltype(std::chrono::seconds(1))& duration = std::chrono::seconds(600)) {
        std::unique_lock lk(mutex_);
        return cv_.wait_for(lk, duration, [this]()->bool { return status() == Status::DONE || status() == Status::TIMEOUT; });
    }

    void setStatus(Status status) {
        status_ = status;
    }

    Status status() const {
        return status_;
    }

    virtual std::string DebugString() const {
        return "TimeoutTaskItem";
    }

 private:
    // non-copyable
    TimeoutTaskItem(const TimeoutTaskItem&) = delete;
    TimeoutTaskItem& operator=(const TimeoutTaskItem&) = delete;

    std::unique_ptr<Timer>  timer_;
    Status                  status_{Status::WAITTING};
    mutable std::mutex      mutex_;
    std::condition_variable cv_;
};

/**
 * @brief 带超时功能的工作队列
 *        队列里的task若超时时间得不到调度会自动被删除
 *
 * @tparam Queue  底层队列类型，默认用 boost::circular_buffer
 */
template <typename Queue = boost::circular_buffer<TimeoutTaskItem::pointer>>
class TimeoutTaskQueue {
 public:
     /**
      * @brief constructor of TimeoutTaskQueue
      *
      * @param capacity        队列最大容量
      * @param timeout_in_ms   task超时时间，超时后会被删除
      * @param n_io_threads    worker threads for timeout handler
      */
    TimeoutTaskQueue(std::size_t capacity, int32_t timeout_in_ms, int32_t n_workers, int32_t n_io_threads = 10)
        : queue_(capacity), n_workers_(n_workers), timeout_(timeout_in_ms) {
        assert(timeout_ > 0);
        assert(n_workers_ > 0);

        io_service_.reset(new IoService);
        io_service_work_.reset(new IoServiceWork(std::ref(*io_service_)));

        for (int32_t i = 0; i < n_io_threads; ++i) {
            io_service_workers_.emplace_back([this] {
                io_service_->run();
            });
        }

        // remove timeout tasks
        cleaner_interval_ = 10 * timeout_;
        cleaner_ = std::thread([&] {
            while (io_service_work_) {
                std::this_thread::sleep_for(std::chrono::milliseconds(cleaner_interval_));
                std::unique_lock lk(mutex_);
                auto new_end = std::remove_if(queue_.begin(), queue_.end(),
                                              [](const TimeoutTaskItem::pointer& item) -> bool {
                                                  return item->status() ==
                                                         TimeoutTaskItem::Status::TIMEOUT;
                                              });
                fprintf(stderr, "Cleanning up queue, removed %lu timeout tasks.\n",
                        std::distance(new_end, queue_.end()));  // DEBUG
                queue_.erase(new_end, queue_.end());
            }  // while
        });
    }

    ~TimeoutTaskQueue() {
        clear();
        io_service_work_.reset();
        io_service_->stop();

        for (auto& thr : io_service_workers_) {
            thr.join();
        }

        cleaner_.join();
    }

    /**
     * @brief   push task item to queue
     *
     * @return  true on success, false on queue is full
     */
    bool push(TimeoutTaskItem::pointer item) {
        std::unique_lock lk(mutex_);

        if (queue_.full()) {
            return false;
        }

        // start timeout counter down before push into the queue
        item->startTimer(*io_service_, timeout_);
        queue_.push_back(std::move(item));
        lk.unlock();
        cv_.notify_one();

        return true;
    }

    /**
     * @brief    pop a task item from queue
     *
     * @return   a task item whose status is not TIMEOUT
     *           ALWAYS check the returned item is nullptr or not
     */
    TimeoutTaskItem::pointer pop() {
        while (true) {
            std::unique_lock lk(mutex_);
            while (queue_.empty()) {
                cv_.wait(lk);
            }

            while (!queue_.empty()) {
                auto item = std::move(queue_.front());
                queue_.pop_front();
                if (!item) {
                    return item; // push null item means mannually terminating
                }
                if (item->status() != TimeoutTaskItem::Status::TIMEOUT) {
                    // stop timer and reset status before being scheduled
                    item->stopTimer();
                    item->setStatus(TimeoutTaskItem::Status::RUNNING);
                    return item;
                }
            }
        }

        return TimeoutTaskItem::pointer();
    }

    void start() {
        workers_.clear();

        for (int32_t i = 0; i < n_workers_; ++i) {
            workers_.emplace_back([this]{
                fprintf(stderr, "TaskQueue worker %ld running...\n", std::this_thread::get_id());  // DEBUG
                while (true) {
                    auto task = pop();
                    if (!task) { return; }
                    if (task && task->status() == common::TimeoutTaskItem::Status::RUNNING) {
                        fprintf(stderr, "Worker %ld running task %s\n", std::this_thread::get_id(), task->DebugString().c_str());  // DEBUG
                        task->doJob();
                    }
                }
            });
        }
    }

    void stop() {
        std::unique_lock lk(mutex_);
        queue_.clear();
        for (int32_t i = 0; i < n_workers_; ++i) {
            queue_.push_back(nullptr);
        }
        cv_.notify_all();
        lk.unlock();

        for (auto& worker : workers_) {
            try {
                worker.join();
            } catch (const std::exception& ex) {
                fprintf(stderr, "TaskQueue worker join exception: %s\n", ex.what());
            }
        }

        fprintf(stderr, "TimeoutTaskQueue terminated.\n");
    }

    void clear() {
        std::unique_lock lk(mutex_);
        queue_.clear();
    }

    std::size_t size() const {
        std::unique_lock lk(mutex_);
        return queue_.size();
    }

    void setCleanInterval(int32_t interval_in_ms) {
        cleaner_interval_ = interval_in_ms;
    }

    std::string DebugString() const {
        std::unique_lock lk(mutex_);
        return boost::str(boost::format("TimeoutTaskQueue: {size:%lu, capacity:%lu, timeout:%d, n_workers:%d, cleaner_interval:%d}")
               % queue_.size() % queue_.capacity() % timeout_ % n_workers_ % cleaner_interval_);
    }

 private:
    Queue   queue_;
    int32_t timeout_ = 0;  // in milliseconds
    int32_t cleaner_interval_ = 0;  // clean timeout items in queue
                                    // for every specified interval milliseconds

    std::unique_ptr<IoService>     io_service_;
    std::unique_ptr<IoServiceWork> io_service_work_;
    std::vector<std::thread>       io_service_workers_;
    std::thread                    cleaner_;

    int32_t                     n_workers_ = 0;
    std::vector<std::thread>    workers_;

    mutable std::mutex      mutex_;
    std::condition_variable cv_;
};

}  // namespace common

using namespace common;

class TestTaskItem : public TimeoutTaskItem {
 public:
    TestTaskItem(std::size_t no) : no_(no) {}

    std::string DebugString() const override {
        return fmt::format("Task {}, status {}", no_, static_cast<int32_t>(status()));
    }

    void onTimeout() override {
        LOG(INFO) << "Task " << no_ << " timeout!";
    }

 private:
    std::size_t no_ = 0;
};

void test_TimeoutTaskQueue_1() {
    TimeoutTaskQueue que(10, 5000);

    for (int i = 0; i < 15; ++i) {
        TimeoutTaskItem::pointer item = std::make_unique<TestTaskItem>(i);
        if (!que.push(std::move(item))) {
            LOG(INFO) << "que is full";
        }
    }  // for i

    LOG(INFO) << "que size is " << que.size();
    // ::sleep(13);
    // LOG(INFO) << "que size is " << que.size();

    while (auto item = que.pop()) {
        LOG(INFO) << "Processing " << item->DebugString();
    }
}

void test_TimeoutTaskQueue_2() {
    TimeoutTaskQueue que(10, 5000);
    LOG(INFO) << que.DebugString();

    // TimeoutTaskItem::pointer item = std::make_unique<TestTaskItem>(1);
    // que.push(std::move(item));

    // ::sleep(6);
    // item = que.pop();
    // if (item) {
    // LOG(INFO) << "Processing " << item->DebugString();
    // } else {
    // LOG(INFO) << "Null item";
    // }
    // return;

    std::thread producer([&] {
        std::size_t no = 0;
        while (true) {
            TimeoutTaskItem::pointer item = std::make_unique<TestTaskItem>(++no);
            if (!que.push(std::move(item))) {
                // LOG(INFO) << "que is full";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    std::thread consumer([&] {
        while (true) {
            auto item = que.pop();
            if (item) {
                LOG(INFO) << "Processing item " << item->DebugString();
            }
            // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }
    });

    producer.join();
    consumer.join();
}

int main() {
    google::LogToStderr();

    // test_TimeoutTaskQueue_1();
    test_TimeoutTaskQueue_2();

    return 0;
}
