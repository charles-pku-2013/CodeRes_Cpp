/*
https://github.com/charlotteLive/pybind11-Chinese-docs/blob/main/src/08.%E7%B1%BB.md

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
 */
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Animal {
 public:
    virtual ~Animal() {}

    virtual std::string go(int n_times) = 0;
};

class Dog : public Animal {
 public:
    std::string go(int n_times) override {
        std::string result;
        for (int i = 0; i < n_times; ++i) result += "woof! ";
        return result;
    }
};

// 为了在python中自定义Animal子类
class PyAnimal : public Animal {
 public:
    /* Inherit the constructors */
    using Animal::Animal;

    /* Trampoline (need one for each virtual function) */
    std::string go(int n_times) override {
        PYBIND11_OVERRIDE_PURE(std::string, /* Return type */
                               Animal,      /* Parent class */
                               go,          /* Name of function in C++ (must match Python name) */
                               n_times      /* Argument(s) */
        );
    }
};

std::string call_go(Animal* animal) {
    return animal->go(3);
}

PYBIND11_MODULE(example, m) {
    py::class_<Animal, PyAnimal /* <--- trampoline*/>(m, "Animal")
        .def(py::init<>())
        .def("go", &Animal::go);

    py::class_<Dog, Animal>(m, "Dog").def(py::init<>());

    m.def("call_go", &call_go);
}

#if 0 // test
from example import *

d = Dog()
call_go(d)     # u'woof! woof! woof! '

class Cat(Animal):
    def go(self, n_times):
        return "meow! " * n_times

c = Cat()
call_go(c)   # u'meow! meow! meow! '
#endif
