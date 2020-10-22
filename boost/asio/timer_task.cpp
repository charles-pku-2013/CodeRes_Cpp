#pragma once
#include <glog/logging.h>
#include <boost/asio.hpp>
#include <memory>
#include <thread>

class TimerTask {
 public:
    using Timer = boost::asio::deadline_timer;
    using IoServiceWork = boost::asio::io_service::work;
    using IoService = boost::asio::io_service;

 public:
    virtual ~TimerTask() { Stop(); }
    virtual void OnTimer() = 0;
    void Start(int interval);
    void Stop();

 protected:
    void _do_timer_job();

    int interval_ = 0;
    bool running_ = false;
    std::unique_ptr<IoService> io_service_;
    std::unique_ptr<IoServiceWork> io_service_work_;
    std::unique_ptr<Timer> timer_;
    std::unique_ptr<std::thread> work_thread_;  // timer job run in single thread
};

inline
void TimerTask::Start(int interval) {
    if (running_) { return; }
    running_ = true;

    interval_ = interval;
    io_service_.reset(new IoService);
    io_service_work_.reset(new IoServiceWork(std::ref(*io_service_)));
    work_thread_.reset(new std::thread([&, this]{
        io_service_->run();
    }));
    timer_.reset(new Timer(*io_service_));
    timer_->expires_from_now(boost::posix_time::seconds(interval_));
    timer_->async_wait(std::bind(&TimerTask::_do_timer_job, this));
}

inline
void TimerTask::Stop() {
    if (!running_) { return; }
    running_ = false;

    timer_->cancel();
    io_service_work_.reset();
    io_service_->stop();
    if (work_thread_->joinable())
    { work_thread_->join(); }
}

inline
void TimerTask::_do_timer_job() {
    if (!running_) { return; }

    try {
        OnTimer();
    } catch (const std::exception &ex) {
        LOG(ERROR) << "TimerTask run routine exception: " << ex.what();
    } catch (...) {
        LOG(ERROR) << "TimerTask run routine unknown exception!";
    }

    if (running_) {
        timer_->expires_from_now(boost::posix_time::seconds(interval_));
        timer_->async_wait(std::bind(&TimerTask::_do_timer_job, this));
    }
}

#if 0
class TimerTask {
 public:
    using Timer = boost::asio::deadline_timer;
    using IoServiceWork = boost::asio::io_service::work;
    using IoService = boost::asio::io_service;

 public:
    virtual ~TimerTask() { Stop(); }
    virtual void OnTimer() = 0;
    bool Start(int interval);
    bool Stop();

 protected:
    void _do_timer_job();

    int interval_ = 0;
    std::unique_ptr<IoService> io_service_;
    std::unique_ptr<IoServiceWork> io_service_work_;
    std::unique_ptr<Timer> timer_;
    std::unique_ptr<std::thread> work_thread_;  // timer job run in single thread
};

inline
bool TimerTask::Start(int interval) {
    if (timer_) { return false; }  // already started

    interval_ = interval;
    io_service_.reset(new IoService);
    io_service_work_.reset(new IoServiceWork(std::ref(*io_service_)));
    timer_.reset(new Timer(std::ref(*io_service_)));
    timer_->expires_from_now(boost::posix_time::seconds(interval_));
    timer_->async_wait(std::bind(&TimerTask::_do_timer_job, this));
    work_thread_.reset(new std::thread([&, this]{
        io_service_->run();
    }));

    return true;
}

inline
bool TimerTask::Stop() {
    if (!timer_) { return false; }  // not running

    timer_->cancel();
    timer_.reset();
    io_service_work_.reset();
    io_service_->stop();
    if (work_thread_->joinable())
    { work_thread_->join(); }
    work_thread_.reset();
    io_service_.reset();

    return true;
}

inline
void TimerTask::_do_timer_job() {
    try {
        OnTimer();
    } catch (const std::exception &ex) {
        LOG(ERROR) << "TimerTask run routine exception: " << ex.what();
    } catch (...) {
        LOG(ERROR) << "TimerTask run routine unknown exception!";
    }
    if (timer_) {
        timer_->expires_from_now(boost::posix_time::seconds(interval_));
        timer_->async_wait(std::bind(&TimerTask::_do_timer_job, this));
    }
}
#endif

