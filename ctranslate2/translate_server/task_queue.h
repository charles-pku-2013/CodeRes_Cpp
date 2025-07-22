#pragma once
#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include <thread>

namespace newtranx {

// TaskQueue with timeout

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

    virtual ~TimeoutTaskItem();

    /**
     * @brief 开始超时计时
     *
     * @param io_service
     * @param timeout_in_ms  超时时间，单位毫秒
     */
    void startTimer(IoService& io_service, int32_t timeout_in_ms);

    /**
     * @brief 取消超时计时
     */
    void stopTimer();

    /**
     * @brief timeout handler, implemented by subclasses
     */
    virtual void onTimeout() {}

    /**
     * @brief task 主任务逻辑
     */
    virtual void jobRoutine() = 0;

    virtual void doJob();

    // bool wait(const decltype(std::chrono::seconds(1))& duration = std::chrono::seconds(600));
    bool wait(const std::chrono::milliseconds& duration = std::chrono::milliseconds(600 * 1000));

    void setStatus(Status status) {
        status_ = status;
    }

    Status status() const {
        return status_;
    }

    virtual std::string DebugString() const;

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
 */
class TimeoutTaskQueue {
 public:
    using Queue = boost::circular_buffer<TimeoutTaskItem::pointer>;

 public:
    /**
      * @brief constructor of TimeoutTaskQueue
      *
      * @param capacity        队列最大容量
      * @param timeout_in_ms   task超时时间，超时后会被删除
      * @param n_io_threads    worker threads for timeout handler
      */
    TimeoutTaskQueue(std::size_t capacity, int32_t timeout_in_ms, int32_t n_workers, int32_t n_io_threads = 10);

    ~TimeoutTaskQueue();

    /**
     * @brief   push task item to queue
     *
     * @return  true on success, false on queue is full
     */
    bool push(TimeoutTaskItem::pointer item);

    /**
     * @brief    pop a task item from queue
     *
     * @return   a task item whose status is not TIMEOUT
     *           ALWAYS check the returned item is nullptr or not
     */
    TimeoutTaskItem::pointer pop();

    void start();

    void stop();

    void clear();

    std::size_t size() const;

    void setCleanInterval(int32_t interval_in_ms) {
        cleaner_interval_ = interval_in_ms;
    }

    std::string DebugString() const;

 private:
    Queue   queue_;
    int32_t timeout_ = 0;           // in milliseconds
    int32_t cleaner_interval_ = 0;  // clean timeout items in queue
                                    // for every specified interval milliseconds

    std::unique_ptr<IoService>     io_service_;
    std::unique_ptr<IoServiceWork> io_service_work_;
    std::vector<std::thread>       io_service_workers_;
    std::thread                    cleaner_;

    int32_t                  n_workers_ = 0;
    std::vector<std::thread> workers_;

    mutable std::mutex      mutex_;
    std::condition_variable cv_;
    std::atomic_bool        running_{false};
};

}  // namespace newtranx
