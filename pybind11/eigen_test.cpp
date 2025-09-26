/*
c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) eigen_test.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++17 -undefined dynamic_lookup $(python3 -m pybind11 --includes) -I/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3 eigen_test.cpp -o example$(python3-config --extension-suffix)
 */
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h> // Include for Eigen support
#include <Eigen/Dense>

#include <iostream>

namespace py = pybind11;

Eigen::MatrixXd process_matrix(const Eigen::MatrixXd& input_matrix) {
    // Perform some Eigen operations
    return input_matrix * 2.0;
}

PYBIND11_MODULE(example, m) {
    m.def("process_matrix", &process_matrix, "A function that processes an Eigen matrix.");
}

#if 0
import example
import numpy as np

numpy_matrix = np.array([[1.0, 2.0], [3.0, 4.0]])
eigen_result = example.process_matrix(numpy_matrix)
print(eigen_result)
#endif
