/*
https://github.com/charlotteLive/pybind11-Chinese-docs/blob/main/src/08.%E7%B1%BB.md

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
 */
#include <pybind11/factory.h>

class Example {
public:
    // ...
    virtual ~Example() = default;
};

class PyExample : public Example {
public:
    using Example::Example;
    PyExample(Example &&base) : Example(std::move(base)) {}
};

PYBIND11_MODULE(example, m) {
    py::class_<Example, PyExample>(m, "Example")
        // Returns an Example pointer.  If a PyExample is needed, the Example
        // instance will be moved via the extra constructor in PyExample, above.
        .def(py::init([]() { return new Example(); }))
        // Two callbacks:
        .def(py::init([]() { return new Example(); } /* no alias needed */,
                      []() { return new PyExample(); } /* alias needed */))
        // *Always* returns an alias instance (like py::init_alias<>())
        .def(py::init([]() { return new PyExample(); }));
}
