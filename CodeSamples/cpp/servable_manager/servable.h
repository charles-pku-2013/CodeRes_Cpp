#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <glog/logging.h>
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_format.h"

namespace demo {

/**
 * @brief  servable (model dict) identifier
 * name    name of model or dict
 * version greater number means more latest version
 */
struct ServableId {
    ServableId() = default;
    ServableId(const std::string _name, int64_t _version)
            : name(_name), version(_version) {}

    std::string name;
    int64_t version = 0;

    bool operator<(const ServableId &rhs) const {
        int diff = name.compare(rhs.name);
        if (diff) { return diff < 0; }
        return version > rhs.version;
    }

    std::string DebugString() const {
        return absl::StrCat("{name: ", name, " version: ", version, "}");
    }
};

/**
 * @brief   shared by all versions of a servable
 * num_versions:    num of versions of this servable
 * class_name:      name of class that create this servable instance, like `DemoDict`
 * servable_name:   same as `name` in ServableId
 * meta_file:       servable meta description file path
 * info:            configs read from meta file
 */
struct ServableInfo {
    int num_versions = 1;
    std::string class_name;
    std::string servable_name;
    std::string meta_file;
    std::unordered_map<std::string, std::string> info;

    std::string DebugString() const {
        return absl::StrFormat("{servable:%s, class:%s, num_versions:%d, info:{%s}, meta_file:%s}",
                servable_name, class_name, num_versions, absl::StrJoin(info, ",",
                absl::PairFormatter(":")), meta_file);
    }
};

/**
 * @brief Represent a servable (model, dict, etc...)
 */
class Servable {
 public:
    virtual ~Servable()
    { LOG(INFO) << "Unloading servable " << DebugString(); }

    const ServableId& Id() const { return id_; }

    void SetId(const ServableId &id) { id_ = id; }
    void SetId(const std::string &name, int64_t version) {
        id_.name = name;
        id_.version = version;
    }

    /**
     * @brief  Model / dict load path on disk
     */
    void SetPath(const std::string& path) { path_ = path; }
    const std::string& Path() const { return path_; }

    const std::shared_ptr<ServableInfo>& Info() const { return info_; }
    void SetInfo(std::shared_ptr<ServableInfo> info)
    { info_ = std::move(info); }

    std::string DebugString() const
    { return absl::StrFormat("{name:%s version:%ld path:%s}", Id().name, Id().version, path_); }

 protected:
    ServableId id_;
    std::string path_;
    std::shared_ptr<ServableInfo> info_;
};

/**
 * @brief   template class wrapper of Servable
 *
 * @tparam T  Servable class like model or dict (DemoDict, KnnModel),
 *            any class type canbe wrapped into ServableHandle
 */
template<typename T>   // DemoDict
class ServableHandle : public Servable {
 public:
    ServableHandle(std::shared_ptr<T> pobj)
            : obj_(std::move(pobj)) {}

    T* get() const { return obj_.get(); }
    std::shared_ptr<T> GetServable() const { return obj_; }

    /**
     * @brief  重载指针运算符，可直接用Handle调用T的API
     */
    T& operator*() const { return *get(); }
    T* operator->() const { return get(); }

    operator bool() const { return get() != nullptr; }

 private:
    std::shared_ptr<T> obj_;
};

}  // namespace demo

