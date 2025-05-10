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

    const std::string& Lookup(const std::string &key);

    std::string Description() const;

 private:
    DictType dict_;
};

}  // namespace demo


