#pragma once
#include "kernel.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include <map>
#include <iostream>

namespace example {

class KernelFactory final {
 public:
    using KernelCreator = std::function<Kernel*(const std::string&)>;
    using KernelCreatorMap = std::map<std::string, KernelCreator>;

 public:
    static KernelFactory& Instance() {
        static KernelFactory inst;
        return inst;
    }

    bool RegisterCreator(const std::string &name, KernelCreator creator) {
        std::cout << "KernelFactory registering creator for " << name << std::endl;  // glog will core here
        if (creator) { creators_[name] = std::move(creator); }
        return true;
    }

    Kernel* Create(const std::string& class_name, const std::string& kernel_name) {
        auto it = creators_.find(class_name);
        return (it != creators_.end() ? (it->second)(kernel_name) : nullptr);
    }


    std::string DebugString() const {
        std::string str_map = absl::StrJoin(creators_, ",",
            [](std::string* out, const KernelCreatorMap::value_type& in) {
                absl::StrAppend(out, in.first);
            });
        return absl::StrFormat("{%s}", str_map);
    }

 private:
    KernelFactory() = default;
    KernelFactory(const KernelFactory&) = delete;
    KernelFactory& operator=(const KernelFactory&) = delete;

    KernelCreatorMap creators_;
};

#define _REGISTER_KERNEL_FACTORY_HELPER_2(x, y) x##y
#define _REGISTER_KERNEL_FACTORY_HELPER(x, y) _REGISTER_KERNEL_FACTORY_HELPER_2(x, y)
#define REGISTER_KERNEL_FACTORY(class_name, type) \
    static bool _REGISTER_KERNEL_FACTORY_HELPER(_register_kernel_factory_dummy, __LINE__) = \
        example::KernelFactory::Instance().RegisterCreator(class_name, \
                [](const std::string& name)-> type* { return new type(name); });
// NOTE 宏参数不要和其他参数如lambda函数形参重名
// 之前宏形参name与lambda函数中的name冲突，改成class_name解决

}  // namespace example


