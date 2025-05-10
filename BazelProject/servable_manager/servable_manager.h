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

namespace demo {

/**
 * @brief Servable管理类, 提供服务发现, 在线更新, 查询获取等功能
 */
class ServableManager : public boost::shared_lockable_adapter<boost::shared_mutex>,
                        public TimerTask {
 public:
    using _VersionTable = std::map<int64_t, std::shared_ptr<Servable>, std::greater<int64_t>>;

    struct _ServableTable {
        std::shared_ptr<ServableInfo> servable_info;
        _VersionTable versions;
    };

    // Servable主表类型 {name : {version : servable}}
    using ServableTable = std::unordered_map<std::string, _ServableTable>;

    // 只保留名称和版本号，方便比较
    using ServableIndexTable = std::map<std::string, std::set<int64_t, std::greater<int64_t>>>;

    /**
     * @brief  Servable构建函数类型，由具体类(like DemoDict)提供实现
     * 根据ServableInfo的信息构造Servable(ServableHandle<T>)，存放到shared_ptr里
     */
    using ServableCreator = std::function<std::pair<bool, std::string>(
                            const std::string&,                     // [in] path
                            const std::shared_ptr<ServableInfo>&,   // [in] ServableInfo
                            std::shared_ptr<Servable>*)>;           // [out] created servable (ServableHandle<T>)

    /**
     * @brief 数据完整性检查函数类型，由具体类提供实现
     */
    using ServableChecker = std::function<std::pair<bool, std::string>(const std::string& /* path */)>;

    // key is class name
    using ServableCreatorTable = std::unordered_map<std::string, ServableCreator>;
    using ServableCheckerTable = std::unordered_map<std::string, ServableChecker>;

    // state during updating
    enum ServableState : int {
        OFFLINE,   // 扫描到但是还未加载
        ONLINE,    // 已经完成加载的或者原本存在的
        FAIL       // 加载失败的
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

    /**
     * @brief 获取servable实例
     *
     * @tparam T         Servable type like DemoDict, KnnModel
     * @param name       Servable name, same as `name` in ServableId
     * @param version    same as `version` in ServableId, 0 (default) means the latest version
     *
     * @return  servable具体类的weak_ptr实例,需要转换为shared_ptr并确认非空后才能使用
     */
    template<typename T>
    std::weak_ptr<T> GetServable(const std::string &name, int64_t version = 0) {
        boost::shared_lock<ServableManager> lck(*this);
        auto it1 = servables_.find(name);
        if (it1 == servables_.end()) { return std::weak_ptr<T>(); }
        auto& inner_map = it1->second.versions;
        auto it2 = (version <= 0 ? inner_map.begin() : inner_map.find(version));
        if (it2 == inner_map.end()) { return std::weak_ptr<T>(); }
        auto handle = dynamic_cast<ServableHandle<T>*>(it2->second.get());
        return (handle ? handle->GetServable() : std::weak_ptr<T>());
    }

    /**
     * @brief 注册具体servable的构造函数 由servable类如DemoDict通过宏`REGISTER_SERVABLE_CREATOR`调用
     *
     * @param name     servable name
     * @param creator  function object that create the servable
     */
    bool RegisterCreator(const std::string &name, ServableCreator creator) {
        boost::unique_lock<ServableManager> lck(*this);
        servable_creators_[name] = std::move(creator);
        return true;
    }

    /**
     * @brief 注册具体servable的数据完整性检查函数 由servable类如DemoDict通过宏`REGISTER_SERVABLE_CHECKER`调用
     *
     * @param name     servable name
     * @param creator  function object that create the servable
     */
    bool RegisterChecker(const std::string &name, ServableChecker checker) {
        boost::unique_lock<ServableManager> lck(*this);
        servable_checkers_[name] = std::move(checker);
        return true;
    }

    /**
     * @brief 启动servable发现主程序
     *
     * @param interval 每隔多久扫描一次磁盘目录
     */
    void Start(int interval = 15);

    /**
     * @brief 终止servable发现程序
     */
    void Stop();

    // 定时器任务
    void OnTimer() override;

    /**
     * @brief 添加servable发现监控路径
     */
    void AddMonitorPath(const std::string &path)
    { monitor_paths_.insert(path); }

    template<typename Iter>
    void AddMonitorPath(Iter beg, Iter end)
    { monitor_paths_.insert(beg, end); }

    // 设置加载servable线程数量
    void SetNumLoadThread(int n)
    { num_load_thread_ = n; }

    // 指定哪些servable必须同时上线
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
    // 根据servable的meta file构建其ServableInfo
    static std::shared_ptr<ServableInfo> _BuildServableInfo(const std::string &servable_name,
                            const std::string& meta_file);

    // 由ServableTable得到只包含name,version的IndexTable，方便比较
    static void _GetServableIndexTable(const ServableTable &in, ServableIndexTable *out);

    static std::string _ServableTable2String(const ServableTable &in);
    static std::string _ServableIndexTable2String(const ServableIndexTable &in);
    static std::string _ServableStateTable2String(const ServableStateTable &in);

    // 扫描monitor_paths_指定的目录集合，发现并更新serable table
    void _ScanForServables();

    // 扫描monitor_paths_指定的单个目录里的servable
    void _ScanDirForServables(const std::string &path);

    // 扫描servable name目录下所有的版本
    void _ScanServableVersions(const std::string &name, const std::string &servable_path);

    bool _LoadServables();  // called by _ScanForServables
    void _CheckGroups();    // called by _ScanForServables

    std::shared_ptr<Servable> _GetServablePtr(const std::string &name, int64_t version) const;

 private:
    ServableTable servables_;                   // 存放servable的主表
    ServableCreatorTable servable_creators_;    // servable的创造者
    ServableCheckerTable servable_checkers_;

    std::set<std::string> monitor_paths_;
    std::list<std::set<std::string>> groups_;

    // for updating
    int num_load_thread_ = 3;
    bool updating_ = false;  // 当前是否在更新流程中
    bool first_run_ = true;  // 是否是第一次运行
    std::unique_ptr<ThreadPool> updating_tp_;   // thread pool for updating
    // 每次扫描时重建以下两个表
    ServableTable found_servables_;             // 新扫描到的servable，要更新到servables_
    ServableStateTable servable_states_;        // 加载时，每个version的状态
    std::atomic_int n_to_load_;
    std::mutex updating_mutex_;
    std::condition_variable updating_cond_;

    ServableManager() = default;
    ServableManager(const ServableManager&) = delete;
    ServableManager(ServableManager&&) = delete;
    ServableManager& operator=(const ServableManager&) = delete;
    ServableManager& operator=(ServableManager&&) = delete;
};

#define _REGISTER_MACRO_HELPER_2(x, y) x##y
#define _REGISTER_MACRO_HELPER(x, y) _REGISTER_MACRO_HELPER_2(x, y)

#define REGISTER_SERVABLE_CREATOR(class_name, type, creator) \
    static bool _REGISTER_MACRO_HELPER(_register_servable_creator_dummy, __LINE__) = \
        demo::ServableManager::Instance().RegisterCreator(class_name, \
                [](const std::string& path, const std::shared_ptr<demo::ServableInfo> &info, \
                    std::shared_ptr<demo::Servable>* pServable)-> \
                    std::pair<bool, std::string> { \
            std::shared_ptr<type> obj; \
            auto status = creator(path, info, &obj); \
            if (!status.first) { \
                *pServable = nullptr; \
                return status; \
            } \
            *pServable = std::make_shared<demo::ServableHandle<type>>(obj); \
            return status; \
        });

#define REGISTER_SERVABLE_CHECKER(class_name, checker) \
    static bool _REGISTER_MACRO_HELPER(_register_servable_checker_dummy, __LINE__) = \
        demo::ServableManager::Instance().RegisterChecker(class_name, checker);

}  // namespace demo

