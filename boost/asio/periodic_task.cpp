#include <string>
#include <memory>
#include <thread>
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <unordered_map>
#include <boost/asio.hpp>


class PeriodicTask {
 public:
    using SharedPtr = std::shared_ptr<PeriodicTask>;
    using WeakPtr = std::weak_ptr<PeriodicTask>;
    virtual void RunSchedule() = 0;
};


class PeriodicTaskSet final : public boost::shared_lockable_adapter<boost::shared_mutex> {
 public:
    using TaskSet = std::unordered_map<std::string, PeriodicTask::WeakPtr>;
    using Timer = boost::asio::deadline_timer;
    using IoServiceWork = boost::asio::io_service::work;
    using IoService = boost::asio::io_service;

 public:
    PeriodicTaskSet(const std::string &_Id) : id_(_Id) {}

    ~PeriodicTaskSet();

    bool Start(int interval);
    bool Stop();

    bool AddTask(const std::string &id, const PeriodicTask::SharedPtr &task) {
        boost::unique_lock<PeriodicTaskSet> lck(*this);
        auto ret = task_set_.emplace(id, task);
        return ret.second;
    }

    bool RemoveTask(const std::string &id) {
        boost::unique_lock<PeriodicTaskSet> lck(*this);
        auto ret = task_set_.erase(id);
        return (ret > 0);
    }

    const std::string& id() const { return id_; }
    std::string DebugString();

 private:
    void _TimerRoutine();

 private:
    std::string id_;
    int interval_ = 0;
    std::unique_ptr<IoService> io_service_;
    std::unique_ptr<IoServiceWork> io_service_work_;
    std::unique_ptr<Timer> timer_;
    std::unique_ptr<std::thread> work_thread_;
    TaskSet task_set_;
};



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
    work_thread_.reset(new std::thread([&,this]{
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
    boost::unique_lock<PeriodicTaskSet> lck(*this);
    for (auto it = task_set_.begin(); it != task_set_.end();) {
        auto p_task = it->second.lock();
        if (p_task) {
            // std::async(std::launch::async, &PeriodicTask::RunSchedule, p_task);  // 经测试不能异步
            // NOTE!!! cannot capture p_task by ref
            std::thread thr([=]{
                try {
                    p_task->RunSchedule();
                } catch (const std::exception &ex) {
                    LOG(ERROR) << "Running periodic task " << it->first << " encounter error: " << ex.what();
                } catch (...) {
                    LOG(ERROR)<< "Running periodic task " << it->first << " encounter unknown error!";
                }
            });
            // std::thread thr(&PeriodicTask::RunSchedule, p_task); // this also OK
            thr.detach();
            ++it;
        } else {  // 对象已经在其他地方析构
            // LOG(INFO) << "Removing task " << it->first;
            it = task_set_.erase(it);
        }
    }  // for it
    lck.unlock();

    // reset timer
    timer_->expires_from_now(boost::posix_time::seconds(interval_));
    timer_->async_wait(std::bind(&PeriodicTaskSet::_TimerRoutine, this));
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


