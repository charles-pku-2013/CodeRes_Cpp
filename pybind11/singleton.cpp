#include <pybind11/pybind11.h>

namespace py = pybind11;

class MySingleton {
public:
    // 获取单例的静态方法
    static MySingleton& getInstance() {
        static MySingleton instance;
        return instance;
    }

    void doSomething() { /* ... */ }

private:
    MySingleton() {} // 私有构造函数
    // 禁止拷贝和赋值
    MySingleton(const MySingleton&) = delete;
    MySingleton& operator=(const MySingleton&) = delete;
};

PYBIND11_MODULE(my_module, m) {
    py::class_<MySingleton>(m, "MySingleton")
        // 将静态方法暴露给 Python
        .def_static("get_instance", &MySingleton::getInstance,
                    py::return_value_policy::reference)
        .def("do_something", &MySingleton::doSomething);
}

// 伪装成 Python 构造函数
PYBIND11_MODULE(my_module, m) {
    py::class_<MySingleton, std::unique_ptr<MySingleton, py::nodelete>>(m, "MySingleton")
        // 绑定构造函数到 getInstance 方法
        .def(py::init([]() {
            return std::unique_ptr<MySingleton, py::nodelete>(&MySingleton::getInstance());
        }))
        .def("do_something", &MySingleton::doSomething);
}

// import my_module
// singleton = my_module.MySingleton.get_instance()
// singleton.do_something()

// import my_module
// # 看起来像创建新对象，实际上获取的是同一个单例
// s1 = my_module.MySingleton()
// s2 = my_module.MySingleton()
// assert s1 is s2

