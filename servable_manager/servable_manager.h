#pragma once
#include "servable.h"
#include "timer_task.h"
#include "thread_pool.h"
#include <map>
#include <set>
#include <list>
#include <memory>
#include <atomic>
#include <condition_variable>
#include <unordered_map>
#include <functional>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread.hpp>
#include <iostream>

namespace common {

class ServableManager : public boost::shared_lockable_adapter<boost::shared_mutex>,
                        public TimerTask {
 public:
    using _VersionTable = std::map<int64_t, std::shared_ptr<Servable>, std::greater<int64_t>>;

    struct _ServableTable {
        std::shared_ptr<ServableInfo> servable_info;
        _VersionTable versions;
    };

    // Servable主表类型
    using ServableTable = std::unordered_map<std::string, _ServableTable>;
    // 只保留名称和版本号，方便比较
    using ServableIndexTable = std::map<std::string, std::set<int64_t, std::greater<int64_t>>>;

    // Servable构建函数类型，由具体类提供实现
    using ServableCreator = std::function<std::pair<bool, std::string>(const std::string&,
            const std::shared_ptr<ServableInfo>&, std::shared_ptr<Servable>*)>;
    // 数据完整性检查函数类型，由具体类提供实现
    using ServableChecker = std::function<std::pair<bool, std::string>(const std::string&)>;
    // key is class name
    using ServableCreatorTable = std::unordered_map<std::string, ServableCreator>;
    using ServableCheckerTable = std::unordered_map<std::string, ServableChecker>;

    // state during updating
    enum ServableState : int {
        OFFLINE, ONLINE, FAIL,
    };

    struct _ServableLoadingState {
        std::string path;
        int state = OFFLINE;
    };

    struct _ServableStateTable {
        std::map<int64_t, _ServableLoadingState, std::greater<int64_t>> versions;
        int num_versions = 0;
    };

    using ServableStateTable = std::unordered_map<std::string, _ServableStateTable>;

 public:
    ~ServableManager() { Stop(); }

    static ServableManager& Instance() {
        static ServableManager inst;
        return inst;
    }

    template<typename T>
    std::shared_ptr<T> GetServable(const std::string &name, int64_t version = 0) {
        boost::shared_lock<ServableManager> lck(*this);
        auto it1 = servables_.find(name);
        if (it1 == servables_.end()) { return nullptr; }
        auto& inner_map = it1->second.versions;
        auto it2 = (version <= 0 ? inner_map.begin() : inner_map.find(version));
        if (it2 == inner_map.end()) { return nullptr; }
        auto handle = dynamic_cast<ServableHandle<T>*>(it2->second.get());
        return (handle ? handle->get_shared() : nullptr);
    }

    bool RegisterCreator(const std::string &name, ServableCreator creator) {
        boost::unique_lock<ServableManager> lck(*this);
        servable_creators_[name] = std::move(creator);
        return true;
    }

    bool RegisterChecker(const std::string &name, ServableChecker checker) {
        boost::unique_lock<ServableManager> lck(*this);
        servable_checkers_[name] = std::move(checker);
        return true;
    }

    void Start(int interval = 15) {
        int old_val = num_load_thread_;
        int n_threads = (int)(std::thread::hardware_concurrency() / 1.5);
        num_load_thread_ = (n_threads > 0 ? n_threads : old_val);  // 第一次启动时全力加载
        updating_tp_.reset();
        first_run_ = true;
        OnTimer();  // 先自己执行一次，之后用定时器执行
        first_run_ = false;
        num_load_thread_ = old_val;
        updating_tp_.reset();
        TimerTask::Start(interval);
    }

    void Stop() {
        updating_ = false;
        if (updating_tp_) {
            updating_tp_->Cancel();
            updating_tp_.reset();
        }
        updating_cond_.notify_all();
        TimerTask::Stop();
        ClearServables();
    }

    // 定时器任务
    void OnTimer() override
    { _ScanForServables(); }

    void AddMonitorPath(const std::string &path)
    { monitor_paths_.insert(path); }

    template<typename Iter>
    void AddMonitorPath(Iter beg, Iter end)
    { monitor_paths_.insert(beg, end); }

    void SetNumLoadThread(int n) { num_load_thread_ = n; }

    template<typename Iter>
    void AddGroup(Iter beg, Iter end) {
        std::set<std::string> group(beg, end);
        if (!group.empty()) {
            groups_.emplace_back();
            groups_.back().swap(group);
        }
    }

    void ClearServables() {
        boost::unique_lock<ServableManager> lck(*this);
        servables_.clear();
    }

    std::string DebugString() const;

 private:  // some helper functions
    static std::shared_ptr<ServableInfo> _BuildServableInfo(const std::string &servable_name,
                            const std::string& meta_file);
    static void _GetServableIndexTable(const ServableTable &in, ServableIndexTable *out);
    static std::string _ServableTable2String(const ServableTable &in);
    static std::string _ServableIndexTable2String(const ServableIndexTable &in);
    static std::string _ServableStateTable2String(const ServableStateTable &in);

    void _ScanForServables();
    void _ScanDirForServables(const std::string &path);
    void _ScanServableVersions(const std::string &name, const std::string &servable_path);
    bool _LoadServables();
    void _CheckGroups();

    std::shared_ptr<Servable> _GetServablePtr(const std::string &name, int64_t version) const {
        auto it1 = servables_.find(name);
        if (it1 == servables_.end()) { return nullptr; }
        auto& inner_map = it1->second.versions;
        auto it2 = inner_map.find(version);
        return (it2 == inner_map.end() ? nullptr : it2->second);
    }

 private:
    ServableTable servables_;                   // 存放servable的主表
    ServableCreatorTable servable_creators_;    // servable的创造者
    ServableCheckerTable servable_checkers_;

    std::set<std::string> monitor_paths_;
    std::list<std::set<std::string>> groups_;

    // for updating
    int num_load_thread_ = 3;
    bool updating_ = false;
    bool first_run_ = true;  // 是否是第一次运行
    std::unique_ptr<ThreadPool> updating_tp_;
    // 每次扫描时重建以下两个表
    ServableTable found_servables_;             // 新扫描到的servable，要更新到servables_
    ServableStateTable servable_states_;        // 加载时，每个version的状态
    std::atomic_int n_to_load_;
    std::mutex updating_mutex_;
    std::condition_variable updating_cond_;

    ServableManager() = default;
    ServableManager(const ServableManager&) = delete;
    ServableManager& operator=(const ServableManager&) = delete;
};

#define _REGISTER_MACRO_HELPER_2(x, y) x##y
#define _REGISTER_MACRO_HELPER(x, y) _REGISTER_MACRO_HELPER_2(x, y)

#define REGISTER_SERVABLE_CREATOR(class_name, type, creator) \
    static bool _REGISTER_MACRO_HELPER(_register_servable_creator_dummy, __LINE__) = \
        common::ServableManager::Instance().RegisterCreator(class_name, \
                [](const std::string& path, const std::shared_ptr<common::ServableInfo> &info, \
                    std::shared_ptr<common::Servable>* pServable)-> \
                    std::pair<bool, std::string> { \
            std::shared_ptr<type> obj; \
            auto status = creator(path, info, &obj); \
            if (!status.first) { \
                *pServable = nullptr; \
                return status; \
            } \
            *pServable = std::make_shared<common::ServableHandle<type>>(obj); \
            return status; \
        });

#define REGISTER_SERVABLE_CHECKER(class_name, checker) \
    static bool _REGISTER_MACRO_HELPER(_register_servable_checker_dummy, __LINE__) = \
        common::ServableManager::Instance().RegisterChecker(class_name, checker);

}  // namespace common

