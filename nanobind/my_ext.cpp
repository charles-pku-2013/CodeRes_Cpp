/*
c++ -O3 -Wall -shared -std=c++17 -undefined dynamic_lookup -I$(python3 -m nanobind --include_dir) my_ext.cpp -o my_ext$(python3-config --extension-suffix)
 */

#include <nanobind/nanobind.h>

int add(int a, int b) { return a + b; }

NB_MODULE(my_ext, m) {
    m.def("add", &add);
}
