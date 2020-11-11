#include "common/periodic/periodic_task.h"
#include <glog/logging.h>
#include <sstream>
#include <future>

PeriodicTaskSet::~PeriodicTaskSet() {
    Stop();
}

bool PeriodicTaskSet::Start(int interval) {
    if (timer_) { return false; }  // already started

    interval_ = interval;
    io_service_.reset(new IoService);
    io_service_work_.reset(new IoServiceWork(std::ref(*io_service_)));
    timer_.reset(new Timer(std::ref(*io_service_)));
    timer_->expires_from_now(boost::posix_time::seconds(interval_));
    timer_->async_wait(std::bind(&PeriodicTaskSet::_TimerRoutine, this));
    work_thread_.reset(new std::thread([&, this]{
        io_service_->run();
    }));

    return true;
}

bool PeriodicTaskSet::Stop() {
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

void PeriodicTaskSet::_TimerRoutine() {
    if (!timer_) { return; }
    boost::unique_lock<PeriodicTaskSet> lck(*this);
    for (auto it = task_set_.begin(); it != task_set_.end();) {
        auto p_task = it->second.lock();
        if (p_task) {
            // NOTE!!! cannot capture p_task by ref
            std::thread thr([p_task]{
                try {
                    p_task->RunSchedule();
                } catch (const std::exception &ex) {
                    LOG(ERROR) << "Running periodic task " << it->first << " encounter error: " << ex.what();
                } catch (...) {
                    LOG(ERROR)<< "Running periodic task " << it->first << " encounter unknown error!";
                }
            });
            thr.detach();
            ++it;
        } else {  // 对象已经在其他地方析构
            // LOG(INFO) << "Removing task " << it->first;
            it = task_set_.erase(it);
        }
    }  // for it
    lck.unlock();

    // reset timer
    if (timer_) {
        timer_->expires_from_now(boost::posix_time::seconds(interval_));
        timer_->async_wait(std::bind(&PeriodicTaskSet::_TimerRoutine, this));
    }
}

std::string PeriodicTaskSet::DebugString() {
    std::ostringstream oss;
    oss << "PeriodicTaskSet " << id_ << " registered tasks: ";
    boost::shared_lock<PeriodicTaskSet> lck(*this);
    for (auto it = task_set_.begin(); it != task_set_.end(); ++it) {
        auto p_task = it->second.lock();
        if (p_task) { oss << it->first << " "; }
    }
    return oss.str();
}

