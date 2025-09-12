/*
https://blog.csdn.net/zhuikefeng/article/details/107224507

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++17 -undefined dynamic_lookup $(python3 -m pybind11 --includes) -I/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3 -I/opt/homebrew/Cellar/fmt/11.2.0/include -L/opt/homebrew/Cellar/fmt/11.2.0/lib -lfmt example.cpp -o example$(python3-config --extension-suffix)
 */
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

void swap(int* a, int* b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin";  // optional module docstring

    m.def("swap", [](py::buffer a, py::buffer b) {
        py::buffer_info a_info = a.request();
        py::buffer_info b_info = b.request();
        swap(static_cast<int*>(a_info.ptr), static_cast<int*>(b_info.ptr));
    });
}
