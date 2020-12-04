#pragma once
#include "kernel.h"

namespace demo {

class Sort : public example::Kernel {
 public:
    explicit Sort(const std::string &name) : example::Kernel(name) {}
    ~Sort();

    int Init() override;
    int Run() override;

    const char* ClassName() const override
    { return "Sort"; }
};

class Rank : public example::Kernel {
 public:
    explicit Rank(const std::string &name) : example::Kernel(name) {}
    ~Rank();

    int Init() override;
    int Run() override;

    const char* ClassName() const override
    { return "Rank"; }
};

}  // namespace demo


