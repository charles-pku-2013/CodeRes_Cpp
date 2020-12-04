/*
# first build so
bazel build //:libplugin.so
# then build demo
bazel build //:demo
# run and input bazel-bin/libplugin.so
bazel-bin/demo
 */
#include "kernel_factory.h"
#include <iostream>
#include <glog/logging.h>
#include <memory>
#include <dlfcn.h>

int main(int argc, char **argv) {
    using namespace std;

    std::string path;
    cout << "Input the so path:" << flush;
    getline(cin, path);

    void *pHandle = NULL;
    pHandle = ::dlopen(path.c_str(), RTLD_LAZY);
    if (!pHandle) {
        cout << "Fail to load so " << path << ". " << ::dlerror() << endl;
        return -1;
    }

    cout << "Successfully loaded lib " << path << endl;
    cout << "KernelFactory: " << example::KernelFactory::Instance().DebugString() << endl;

    std::unique_ptr<example::Kernel> sorter(example::KernelFactory::Instance().Create("Sort", "sorter"));
    if (sorter) {
        sorter->Init();
        sorter->Run();
    } else {
        LOG(ERROR) << "Fail to create sorter";
    }

    std::unique_ptr<example::Kernel> ranker(example::KernelFactory::Instance().Create("Rank", "ranker"));
    if (ranker) {
        ranker->Init();
        ranker->Run();
    } else {
        LOG(ERROR) << "Fail to create ranker";
    }

    // sorter.reset();
    // ranker.reset();
    cout << "Unloading so " << path << endl;
    ::dlclose(pHandle);  // 卸载so后不影响析构函数调用

    return 0;
}
