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
    bool Start(int interval);
    bool Stop();

 protected:
    void _do_timer_job();

    int interval_ = 0;
    std::unique_ptr<IoService> io_service_;
    std::unique_ptr<IoServiceWork> io_service_work_;
    std::unique_ptr<Timer> timer_;
    std::unique_ptr<std::thread> work_thread_;
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
    timer_->expires_from_now(boost::posix_time::seconds(interval_));
    timer_->async_wait(std::bind(&TimerTask::_do_timer_job, this));
}

