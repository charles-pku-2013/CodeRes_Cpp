/*
https://github.com/charlotteLive/pybind11-Chinese-docs/blob/main/src/05.%E9%9D%A2%E5%AF%B9%E5%AF%B9%E8%B1%A1%E7%BC%96%E7%A8%8B.md

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
 */
#include <pybind11/pybind11.h>

namespace py = pybind11;

struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};

PYBIND11_MODULE(example, m) {
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())  // 构造函数
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName)
        .def_readwrite("name", &Pet::name)  // 直接读写成员变量 p.name = "Charly"
        .def("__repr__", [](const Pet &a)->std::string {
            return "<example.Pet named '" + a.name + "'>";
        });
}

#if 0
import example
p = example.Pet("Molly")
print(p)
<example.Pet object at 0x10cd98060>
p.getName()
u'Molly'
p.setName("Charly")
p.getName()
u'Charly'
#endif
