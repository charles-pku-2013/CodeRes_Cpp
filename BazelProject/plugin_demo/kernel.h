#pragma once
#include <string>
#include "absl/strings/str_format.h"

namespace example {

class Kernel {
 public:
    explicit Kernel(const std::string &name) : name_(name) {}

    virtual ~Kernel() = default;

    virtual int Init() = 0;
    virtual int Run() = 0;

    virtual const char* ClassName() const = 0;

    virtual std::string DebugString() const
    { return absl::StrFormat("{name:%s class:%s}", name_, ClassName()); }

 protected:
    std::string name_;
};

}  // namespace example
