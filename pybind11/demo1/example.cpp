#include <pybind11/pybind11.h>

#include <iostream>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

void inadd() {
    int a, b;
    std::cin >> a >> b;
    std::cout << a + b << std::endl;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin";  // optional module docstring

    m.def("add", &add, "A function which adds two numbers");

    m.def("inadd", &inadd, "cin and cout");
}

