#pragma once
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <thread>
#include <unordered_map>


class PeriodicTask {
 public:
    using SharedPtr = std::shared_ptr<PeriodicTask>;
    using WeakPtr = std::weak_ptr<PeriodicTask>;
    virtual ~PeriodicTask() = default;
    virtual void RunSchedule() = 0;
};


class PeriodicTaskSet final : public boost::shared_lockable_adapter<boost::shared_mutex> {
 public:
    using pointer = std::shared_ptr<PeriodicTaskSet>;
    using TaskSet = std::unordered_map<std::string, PeriodicTask::WeakPtr>;
    using Timer = boost::asio::deadline_timer;
    using IoServiceWork = boost::asio::io_service::work;
    using IoService = boost::asio::io_service;

 public:
    explicit PeriodicTaskSet(const std::string &_Id) : id_(_Id) {}

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
        return (task_set_.erase(id) > 0);
    }

    const std::string& Id() const { return id_; }
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


class PeriodicService final : public boost::shared_lockable_adapter<boost::shared_mutex> {
 public:
    using PeriodicDict = std::unordered_map<std::string, PeriodicTaskSet::pointer>;

 public:
    static PeriodicService& Instance() {
        static PeriodicService inst;
        return inst;
    }

    bool NewPeriodicTaskSet(const std::string& id, int interval) {
        boost::unique_lock<PeriodicService> lck(*this);
        if (dict_.count(id)) { return false; }
        auto pPeriodicTaskSet = std::make_shared<PeriodicTaskSet>(id);
        pPeriodicTaskSet->Start(interval);
        dict_[id] = pPeriodicTaskSet;
        return true;
    }

    PeriodicTaskSet::pointer Get(const std::string& id) {
        boost::shared_lock<PeriodicService> lck(*this);
        auto it = dict_.find(id);
        return (it == dict_.end() ? nullptr : it->second);
    }

    bool Remove(const std::string& id) {
        boost::unique_lock<PeriodicService> lck(*this);
        return (dict_.erase(id) > 0);
    }

 private:
    PeriodicService() = default;
    PeriodicService(const PeriodicService&) = delete;
    PeriodicService& operator=(const PeriodicService&) = delete;

    PeriodicDict dict_;
};


