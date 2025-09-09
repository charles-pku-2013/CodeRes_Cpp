/*
https://pybind11.readthedocs.io/en/stable/advanced/cast/stl.html

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++17 -undefined dynamic_lookup $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
 */
#include <vector>
#include <map>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<int>)
PYBIND11_MAKE_OPAQUE(std::map<std::string, double>)

PYBIND11_MODULE(example, m) {
    py::bind_vector<std::vector<int>>(m, "VectorInt");
    py::bind_map<std::map<std::string, double>>(m, "MapStringDouble");
}
