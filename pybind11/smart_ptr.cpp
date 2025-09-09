/*
https://github.com/charlotteLive/pybind11-Chinese-docs/blob/main/src/10.%E6%99%BA%E8%83%BD%E6%8C%87%E9%92%88.md

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) smart_ptr.cpp -o example$(python3-config --extension-suffix)
 */
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Child { };

class Parent {
public:
   Parent() : child(std::make_shared<Child>()) { }
   // Child *get_child() { return child.get(); }  /* Hint: ** DON'T DO THIS ** */
   // malloc: *** error for object 0x600001d40018: pointer being freed was not allocated

   std::shared_ptr<Child> get_child() { return child; }
private:
    std::shared_ptr<Child> child;
};

PYBIND11_MODULE(example, m) {
    py::class_<Child, std::shared_ptr<Child>>(m, "Child");

    py::class_<Parent, std::shared_ptr<Parent>>(m, "Parent")
       .def(py::init<>())
       .def("get_child", &Parent::get_child);
}

#if 0
from example import Parent
print(Parent().get_child())
#endif
