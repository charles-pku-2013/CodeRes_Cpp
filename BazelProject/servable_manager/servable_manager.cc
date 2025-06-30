#include "servable_manager.h"
#include "config_reader.h"
#include <chrono>
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <glog/logging.h>

namespace demo {

namespace fs = boost::filesystem;

void ServableManager::Start(int interval) {
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

void ServableManager::Stop() {
    updating_ = false;
    if (updating_tp_) {
        updating_tp_->Cancel();
        updating_tp_.reset();
    }
    updating_cond_.notify_all();
    TimerTask::Stop();
    ClearServables();
}

void ServableManager::OnTimer()
{ _ScanForServables(); }

std::shared_ptr<Servable>
ServableManager::_GetServablePtr(const std::string &name, int64_t version) const {
    auto it1 = servables_.find(name);
    if (it1 == servables_.end()) { return nullptr; }
    auto& inner_map = it1->second.versions;
    auto it2 = inner_map.find(version);
    return (it2 == inner_map.end() ? nullptr : it2->second);
}

std::shared_ptr<ServableInfo>
ServableManager::_BuildServableInfo(const std::string &servable_name,
                            const std::string& meta_file) {
    ConfigReader cfg_reader;
    if (!cfg_reader.load_config(meta_file)) {
        LOG(ERROR) << "ServableManager fail to build servable info from " << meta_file;
        return nullptr;
    }

    std::shared_ptr<ServableInfo> ret(new ServableInfo);
    ret->servable_name = servable_name;
    ret->meta_file = meta_file;
    ret->info = cfg_reader.all_configs();
    ret->num_versions = cfg_reader.get_config<int>("num_versions", ret->num_versions);
    ret->class_name = cfg_reader.get_config<std::string>("class_name", ret->class_name);

    return ret;
}

void ServableManager::_GetServableIndexTable(const ServableTable &in, ServableIndexTable *out) {
    ServableIndexTable _out;
    for (const auto& kv1 : in) {
        auto& version_set = _out[kv1.first];
        for (const auto& kv2 : kv1.second.versions) {
            version_set.insert(kv2.first);
        }
    }
    out->swap(_out);
}

std::string ServableManager::_ServableIndexTable2String(const ServableIndexTable &in) {
    std::string str_table = absl::StrJoin(in, ",",
            [](std::string *out, const ServableIndexTable::value_type &val){
        absl::StrAppend(out, absl::StrCat(val.first, "{", absl::StrJoin(val.second, ","), "}"));
    });
    return absl::StrCat("{", str_table, "}");
}

std::string ServableManager::_ServableTable2String(const ServableTable &in) {
    ServableIndexTable servables_index;
    _GetServableIndexTable(in, &servables_index);
    return _ServableIndexTable2String(servables_index);
}

std::string ServableManager::_ServableStateTable2String(const ServableStateTable &in) {
    auto state_2_string = [](int state) {
        switch (state) {
            case ONLINE: return "online";
            case OFFLINE: return "offline";
            case FAIL: return "fail";
        }
        return "unknown";
    };

    auto ver_state_table_2_string =
        [&](std::string *out, const decltype(_ServableStateTable::versions)::value_type& val) {
            absl::StrAppend(out, val.first, ":{",
                "path:", val.second.path, ",",
                "state:", state_2_string(val.second.state),
                "}");
    };

    std::string content = absl::StrJoin(in, ", ", [&](std::string *out, const ServableStateTable::value_type& val){
            absl::StrAppend(out, absl::StrCat(val.first, "{",
                absl::StrJoin(val.second.versions, ", ", ver_state_table_2_string), "}"));
    });

    return absl::StrCat("{", content, "}");
}

void ServableManager::_ScanForServables() {
    constexpr int LOCK_TIMEOUT = 10;

    if (updating_) {
        LOG(INFO) << "ServableManager updating: Another updating routine in progress, abort!";
        return;
    }

    // auto cleanup on return
    std::unique_ptr<void, std::function<void(void*)>> _on_finish((void*)1, [this](void*){
        found_servables_.clear();
        updating_ = false;
    });
    updating_ = true;

    boost::shared_lock<ServableManager> slck(*this, boost::defer_lock);
    if (!slck.try_lock()) {
        LOG(ERROR) << "ServableManager updating: failed to get lock, abort!";
        return;
    }

    n_to_load_ = 0;
    found_servables_.clear();
    servable_states_.clear();
    if (!updating_tp_) {
        updating_tp_.reset(new ThreadPool(num_load_thread_));
    }

    // find out all candidate servables (no loading)
    for (const auto &path : monitor_paths_) {
        _ScanDirForServables(path);
    }

    // remove / restore those not meet group requirement
    _CheckGroups();

    // load new servables, maybe take one more time if someone fail
    while (!_LoadServables()) {
        std::unique_lock<std::mutex> lk(updating_mutex_);
        if (!updating_cond_.wait_for(lk, std::chrono::hours(3),
                    [this]{ return (n_to_load_ <= 0 || !updating_); })) {  // wait timeout
            LOG(ERROR) << "ServableManager updating fail for waiting too long!";
            if (updating_tp_) {
                updating_tp_->Cancel();
                updating_tp_.reset();
            }
            return;
        }
        if (!updating_) {
            LOG(INFO) << "ServableManager updating canceled";
            return;
        }
    }

    // remove those not online and trim to num_versions
    for (auto it1 = found_servables_.begin(); it1 != found_servables_.end();) {
        int num_versions = it1->second.servable_info->num_versions;
        auto& ver_table = it1->second.versions;
        int count = 0;
        for (auto it2 = ver_table.begin(); it2 != ver_table.end();) {
            if (!it2->second) {
                it2 = ver_table.erase(it2);
            } else {
                if (++count > num_versions) {
                    it2 = ver_table.erase(it2);
                } else {
                    ++it2;
                }
            }
        }
        if (ver_table.empty()) {
            it1 = found_servables_.erase(it1);
        } else {
            ++it1;
        }
    }

    // TODO 二次版本对齐，处理可能加载失败的情况
    _CheckGroups();

    // DEBUG
    // LOG(INFO) << "After load found_servables_: " << _ServableTable2String(found_servables_);
    // LOG(INFO) << "After load servable_states_: " << _ServableStateTable2String(servable_states_);

    // 判断加载的servable和已有的servable是否相同，
    ServableIndexTable servables_index, found_servables_index;
    _GetServableIndexTable(servables_, &servables_index);
    _GetServableIndexTable(found_servables_, &found_servables_index);
    // no need to update
    if (servables_index == found_servables_index) {
        // LOG(INFO) << "No need to update";  // DEBUG
        return;
    }

    LOG(INFO) << "ServableManager updating: Current servables: " << _ServableIndexTable2String(servables_index)
            << " New servables: " << _ServableIndexTable2String(found_servables_index) << " Updating!";
    auto deadline = boost::chrono::steady_clock::now() + boost::chrono::seconds(LOCK_TIMEOUT);
    boost::unique_lock<ServableManager> ulck(*this, boost::defer_lock);
    slck.unlock();
    if (!ulck.try_lock_until(deadline)) {
        LOG(ERROR) << "ServableManager updating: failed to get exclusive lock for updating, abort!";
        return;
    }
    servables_.swap(found_servables_);
}

void ServableManager::_ScanDirForServables(const std::string &path) {
    // LOG(INFO) << "Scanning for servables in " << path;  // DEBUG
    // 对应 name 级目录
    if (!fs::exists(path)) {
        LOG(WARNING) << "ServableManager updating: monitoring path "
                << path << " does not exist, skipping!";
        return;
    }

    for (fs::directory_iterator itr(path); itr != fs::directory_iterator(); ++itr) {
        if (!fs::is_directory(*itr) || fs::is_empty(*itr)) {
            continue;
        }

        auto& servable_path = itr->path();
        std::string name = servable_path.stem().string();  // servable name
        if (found_servables_.count(name)) {  // 不添加重名的servable
            LOG(ERROR) << absl::StrFormat("ServableManager updating: "
                "skipping servable %s with path %s, for servable with "
                "same name but path is %s already exists!", name, servable_path.string(),
                fs::path(found_servables_[name].servable_info->meta_file).parent_path().string());
            continue;
        }

        auto it_servable = servables_.find(name);
        if (it_servable != servables_.end()) {
            // servable已经存在，复制ServableInfo
            found_servables_[name].servable_info = it_servable->second.servable_info;
        } else {
            // 新servable, 构造ServableInfo
            auto pServableInfo = _BuildServableInfo(name, (servable_path / "meta").string());
            if (!pServableInfo) {
                LOG(WARNING) << "ServableManager updating: scan servables skipping "
                        << servable_path << " for no valid meta found!";
                continue;
            }
            const auto& class_name = pServableInfo->class_name;
            if (class_name.empty()) {
                LOG(ERROR) << "ServableManager updating: scan servables class_name not set in meta " << servable_path;
                continue;
            }
            if (!servable_creators_.count(class_name)) {
                LOG(ERROR) << "ServableManager updating: servable class "
                        << class_name << " not supported! " << servable_path;
                continue;
            }
            LOG(INFO) << "ServableManager updating: Successfully build servable info " << pServableInfo->DebugString();
            found_servables_[name].servable_info = std::move(pServableInfo);
        }

        _ScanServableVersions(name, servable_path.string());
    }  // for itr
}

// 把所有的version号和已经加载的servable copy到容器中
void ServableManager::_ScanServableVersions(const std::string &name, const std::string &servable_path) {
    // name/version 目录
    // LOG(INFO) << "Scanning versions for servable " << name << " in dir " << servable_path;  // DEBUG
    auto& servable_state_table = servable_states_[name];
    servable_state_table.num_versions = found_servables_[name].servable_info->num_versions;
    auto& ver_state_table = servable_state_table.versions;
    for (fs::directory_iterator vitr(servable_path); vitr != fs::directory_iterator(); ++vitr) {
        if (!fs::is_directory(*vitr) || fs::is_empty(*vitr)) {
            continue;
        }
        auto &vpath = vitr->path();  // version dir name like 1514117
        int64_t version = 0;
        if (!boost::conversion::try_lexical_convert(vpath.stem().string(), version) || version <= 0) {
            continue;
        }
        auto pServable = _GetServablePtr(name, version);
        if (!pServable) {
            // name / version servable doesn't exist
            // 数据检查(若有指定)
            auto it_checker = servable_checkers_.find(found_servables_[name].servable_info->class_name);
            if (it_checker != servable_checkers_.end() && it_checker->second) {
                auto status = (it_checker->second)(vpath.string());
                if (!status.first) {
                    LOG(WARNING) << absl::StrFormat("ServableManager updating: skipping %s "
                        "because of check fail: %s", vpath.string(), status.second);
                    continue;
                }
            }
        }  // if !pServable
        auto& state = ver_state_table[version];
        state.path = vpath.string();
        state.state = (pServable ? ONLINE : OFFLINE);
        found_servables_[name].versions[version] = pServable;  // 从旧表中复制已有的servable
    }
}

bool ServableManager::_LoadServables() {
    bool ok = true;
    for (auto& table : servable_states_) {
        const std::string& name = table.first;
        int num_versions = table.second.num_versions;
        auto& ver_table = table.second.versions;
        int count = 0;

        // 一个词典的加载
        for (auto it = ver_table.begin(); it != ver_table.end(); ++it) {
            int64_t version = it->first;
            auto& st = it->second;
            auto& path = st.path;
            if (st.state == FAIL) {
                // skip
                continue;
            } else if (st.state == ONLINE) {
                // skip already exist
                if (++count >= num_versions) {
                    break;
                }
            } else {  // OFFLINE create new servable
                ++n_to_load_;
                ok = false;
                auto& servable_info = found_servables_[name].servable_info;
                assert(servable_info);
                auto& creator = servable_creators_[servable_info->class_name];
                assert(creator);
                auto& pServable = found_servables_[name].versions[version];

                updating_tp_->Schedule([this, &creator, path, &servable_info, name, version, &pServable]{
                    LOG(INFO) << absl::StrFormat("ServableManager updating: Loading servable {name:%s, version:%ld, path:%s}...",
                            name, version, path);
                    std::pair<bool, std::string> status;
                    try {
                        status = creator(path, servable_info, &pServable);
                    } catch (const std::exception &ex) {
                        pServable = nullptr;
                        status = std::make_pair(false, ex.what());
                    } catch (...) {
                        pServable = nullptr;
                        status = std::make_pair(false, "Unknown exception");
                        throw;  // NOTE! 如果当前线程被pthread_cancel杀死，必须rethrow exception 运行时报错 FATAL: exception not rethrown
                    }
                    if (status.first) {
                        pServable->SetId(name, version);
                        pServable->SetPath(path);
                        pServable->SetInfo(servable_info);
                        servable_states_[name].versions[version].state = ONLINE;
                        LOG(INFO) << "ServableManager updating: Successfully loaded servable " << pServable->DebugString();
                    } else {
                        servable_states_[name].versions[version].state = FAIL;
                        if (first_run_) {
                            // 刚启动时所有servable都要加载成功，否则直接退出
                            LOG(FATAL) << absl::StrFormat("ServableManager updating: "
                                "Create servable {name:%s, version:%ld, path:%s} fail: %s",
                                name, version, path, status.second);
                        } else {
                            LOG(ERROR) << absl::StrFormat("ServableManager updating: "
                                "Create servable {name:%s, version:%ld, path:%s} fail: %s",
                                name, version, path, status.second);
                        }
                    }
                    if (--n_to_load_ <= 0)
                    { updating_cond_.notify_all(); }
                });

                if (++count >= num_versions) {
                    break;
                }
            }  // if OFFLINE
        }  // for it
    }
    return ok;
}

// 把需要同时加载的词典版本对齐
// 第一次启动检查group内各servable version的交集
void ServableManager::_CheckGroups() {
    auto get_ver_set = [this](const std::string &name, int& min_num_versions, std::vector<int64_t> *ver_set) {
        auto it = found_servables_.find(name);
        if (it == found_servables_.end()) {
            ver_set->clear();
            return;
        }
        std::vector<int64_t> _ver_set;
        _ver_set.reserve(32);
        for (auto& kv : it->second.versions) {
            _ver_set.emplace_back(kv.first);
        }
        ver_set->swap(_ver_set);
        int num_versions = it->second.servable_info->num_versions;
        if (num_versions < min_num_versions) {
            min_num_versions = num_versions;
        }
    };

    constexpr int MAX = 65535;
    for (const auto& group : groups_) {
        // got common version set
        // LOG(INFO) << absl::StrFormat("Checking group {%s}", absl::StrJoin(group, ","));  // DEBUG
        std::vector<int64_t> comm_ver;
        int min_num_versions = MAX;
        bool first = true;
        for (const auto& servable_name : group) {
            std::vector<int64_t> ver_set;
            get_ver_set(servable_name, min_num_versions, &ver_set);
            if (first) {
                comm_ver.swap(ver_set);
                first = false;
            } else {
                std::vector<int64_t> tmp_comm_ver;
                tmp_comm_ver.reserve(32);
                std::set_intersection(comm_ver.begin(), comm_ver.end(),
                        ver_set.begin(), ver_set.end(), std::back_inserter(tmp_comm_ver), std::greater<int64_t>());
                comm_ver.swap(tmp_comm_ver);
            }
        }  // got common_ver of a group
        // LOG(INFO) << absl::StrFormat("ServableManager updating: group {%s} has common candidate versions {%s} "
                // "min_num_versions: %d", absl::StrJoin(group, ","), absl::StrJoin(comm_ver, ","), min_num_versions);  // DEBUG
        // trim found_servables_
        std::set<int64_t, std::greater<int64_t>> comm_ver_set(comm_ver.begin(), comm_ver.end());
        for (const auto& servable_name : group) {
            auto it = found_servables_.find(servable_name);
            if (it == found_servables_.end()) {
                continue;
            }
            // reset num_versions
            if (min_num_versions != MAX) {
                it->second.servable_info->num_versions = min_num_versions;
            }
            auto& ver_table = it->second.versions;
            for (auto ver_it = ver_table.begin(); ver_it != ver_table.end();) {
                if (!comm_ver_set.count(ver_it->first)) {
                    LOG(INFO) << absl::StrFormat("ServableManager updating: _CheckGroups removing (candidate) servable {%s:%ld}",
                            servable_name, ver_it->first);
                    servable_states_[servable_name].versions.erase(ver_it->first);
                    ver_it = ver_table.erase(ver_it);
                } else {
                    ++ver_it;
                }
            }  // for ver_it
            if (ver_table.empty()) {
                LOG(INFO) << "ServableManager updating: _CheckGroups removing all (candidate) versions of servable: " << servable_name;  // DEBUG
                servable_states_.erase(servable_name);
                found_servables_.erase(it);
            }
        }
    }  // for group
}

std::string ServableManager::DebugString() const {
    auto* _this = const_cast<ServableManager*>(this);
    boost::shared_lock<ServableManager> lck(*_this);

    std::string str_creators = absl::StrJoin(servable_creators_, ",",
        [](std::string* out, const ServableCreatorTable::value_type& in) {
            absl::StrAppend(out, in.first);
        });

    // std::string str_servables = absl::StrJoin(servables_, ",",
        // [](std::string* out, const ServableTable::value_type& val) {
            // absl::StrAppend(out, absl::StrCat(val.first, "{",
                    // absl::StrJoin(val.second.versions, ",", [](std::string *out, const _VersionTable::value_type& val) {
                        // absl::StrAppend(out, val.first);
                    // }), "}"));
        // });

    std::string str_servables = _ServableTable2String(servables_);

    std::string str_monitor_paths = absl::StrJoin(monitor_paths_, ",");

    std::string str_groups = absl::StrJoin(groups_, ",",
        [](std::string *out, const std::set<std::string> &val){
            absl::StrAppend(out, absl::StrCat("{", absl::StrJoin(val, ","), "}"));
        });

    return absl::StrFormat("{classes:{%s} servables:%s monitor_paths:{%s} num_load_thread:%d groups:{%s}}",
            str_creators, str_servables, str_monitor_paths, num_load_thread_, str_groups);
}

}  // namespace demo

