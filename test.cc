
#include <pybind11/pybind11.h>
#include <boost/interprocess/sync/named_recursive_mutex.hpp>
#include <cstdlib>
#include <mutex>
#include <string>
#include <iostream>

namespace py = pybind11;
using namespace boost::interprocess;

class NamedMutexManager {
private:
    // 静态成员：存储所有实例共享的元数据
    static std::string target_name;
    static std::once_flag register_flag;

    // 静态清理函数：匹配 std::atexit 所需的 void(*)() 签名
    static void static_cleanup() {
        if (!target_name.empty()) {
            // 执行实际的删除操作
            bool removed = named_recursive_mutex::remove(target_name.c_str());
            if (removed) {
                std::cout << "[C++] Mutex " << target_name << " removed successfully." << std::endl;
            }
        }
    }

public:
    NamedMutexManager(std::string name) {
        // 1. 设置静态变量（仅在第一次或根据逻辑覆盖）
        target_name = name;

        // 2. 确保 std::atexit 只被调用一次
        std::call_once(register_flag, []() {
            // 使用非捕获 lambda 包装静态函数
            std::atexit(static_cleanup);
            std::cout << "[C++] Registered atexit cleanup for: " << target_name << std::endl;
        });

        // 3. 初始化互斥量逻辑
        // named_recursive_mutex mtx(open_or_create, target_name.c_str());
    }
};

// 必须在类外部初始化静态成员
std::string NamedMutexManager::target_name = "";
std::once_flag NamedMutexManager::register_flag;

// pybind11 模块定义
PYBIND11_MODULE(my_mutex_module, m) {
    py::class_<NamedMutexManager>(m, "NamedMutexManager")
        .def(py::init<std::string>());
}
