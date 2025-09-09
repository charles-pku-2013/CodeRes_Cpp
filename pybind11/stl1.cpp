/*
https://github.com/charlotteLive/pybind11-Chinese-docs/blob/main/src/10.%E6%99%BA%E8%83%BD%E6%8C%87%E9%92%88.md

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>

namespace py = pybind11;

PYBIND11_MODULE(example, m) {
    // bind lambda function
    m.def("print_vector", [](const auto& v) {
        for (auto& item : v) {
            std::cout << item << " " << std::flush;
        }
        std::cout << std::endl;
    });
}

