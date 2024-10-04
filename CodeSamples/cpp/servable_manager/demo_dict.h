#pragma once
#include <string>
#include <unordered_map>

namespace demo {

class DemoDict final {
 public:
    using DictType = std::unordered_map<std::string, std::string>;

 public:
    ~DemoDict();

    bool Load(const std::string &fname);

    const std::string& Lookup(const std::string &key) {
        static std::string null;
        auto it = dict_.find(key);
        return (it == dict_.end() ? null : it->second);
    }

 private:
    DictType dict_;
};

}  // namespace demo


