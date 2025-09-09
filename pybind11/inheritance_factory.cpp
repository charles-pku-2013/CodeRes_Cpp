/*
https://github.com/charlotteLive/pybind11-Chinese-docs/blob/main/src/05.%E9%9D%A2%E5%AF%B9%E5%AF%B9%E8%B1%A1%E7%BC%96%E7%A8%8B.md

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) inheritance.cpp -o example$(python3-config --extension-suffix)
 */
#include <pybind11/pybind11.h>

namespace py = pybind11;

struct Pet {
    Pet(const std::string& name) : name(name) {}

    virtual ~Pet() = default;

    void set(int age_) {
        age = age_;
    }

    void set(const std::string& name_) {
        name = name_;
    }

    std::string name;
    int         age;
};

struct Dog : Pet {
    Dog(const std::string& name) : Pet(name) {}

    std::string bark() const {
        return "woof!";
    }
};

PYBIND11_MODULE(example, m) {
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string&>()).def_readwrite("name", &Pet::name);
        // 函数重载
        .def("set", static_cast<void (Pet::*)(int)>(&Pet::set), "Set the pet's age")
        .def("set", static_cast<void (Pet::*)(const std::string&)>(&Pet::set),
             "Set the pet's name");
        // or
        // .def("set", py::overload_cast<int>(&Pet::set), "Set the pet's age")
        // .def("set", py::overload_cast<const std::string &>(&Pet::set), "Set the pet's name");

    // Method 1: template parameter:
    py::class_<Dog, Pet /* <- specify C++ parent type */>(m, "Dog")
        .def(py::init<const std::string&>())
        .def("bark", &Dog::bark);

    // Method 2: pass parent class_ object:
    // py::class_<Dog>(m, "Dog", pet [> <- specify Python parent type <])
        // .def(py::init<const std::string &>())
        // .def("bark", &Dog::bark);

    // 返回一个指向派生类的基类指针
    m.def("pet_store", []() { return std::unique_ptr<Pet>(new Dog("Molly")); });
}

#if 0
struct Widget {
    int foo(int x, float y);
    int foo(int x, float y) const;
};

py::class_<Widget>(m, "Widget")
   .def("foo_mutable", py::overload_cast<int, float>(&Widget::foo))
   .def("foo_const",   py::overload_cast<int, float>(&Widget::foo, py::const_));
#endif
