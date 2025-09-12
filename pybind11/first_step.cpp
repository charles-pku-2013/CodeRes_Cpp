/*
https://github.com/charlotteLive/pybind11-Chinese-docs/blob/main/src/04.%E9%A6%96%E6%AC%A1%E5%B0%9D%E8%AF%95.md

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++17 -undefined dynamic_lookup $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
 */
#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("add", &add, "A function which adds two numbers", py::arg("i") = 3, py::arg("j") = 5);
}

#if 0
>>> import example
>>> example.add(1, 2)
    example.add(i=1, j=2)
3L
>>>
#endif
