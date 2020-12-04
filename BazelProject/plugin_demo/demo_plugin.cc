#include "demo_plugin.h"
#include "kernel_factory.h"
#include <glog/logging.h>

namespace demo {

Sort::~Sort() {
    LOG(INFO) << "Kernel " << DebugString() << " destruct";
}

int Sort::Init() {
    LOG(INFO) << "Kernel " << DebugString() << " init...";
    return 0;
}

int Sort::Run() {
    LOG(INFO) << "Kernel " << DebugString() << " run...";
    return 0;
}

Rank::~Rank() {
    LOG(INFO) << "Kernel " << DebugString() << " destruct";
}

int Rank::Init() {
    LOG(INFO) << "Kernel " << DebugString() << " init...";
    return 0;
}

int Rank::Run() {
    LOG(INFO) << "Kernel " << DebugString() << " run...";
    return 0;
}

REGISTER_KERNEL_FACTORY("Sort", Sort)
REGISTER_KERNEL_FACTORY("Rank", Rank)

}  // namespace demo

