// C++ side (my_module.cpp)
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <vector>

namespace py = pybind11;

// Example function in C++ that takes a list of NumPy arrays
void process_numpy_arrays(const std::vector<py::array_t<double>>& numpy_arrays_list) {
    for (const auto& arr : numpy_arrays_list) {
        // Access array data and dimensions
        py::buffer_info buf_info = arr.request();
        double* data = static_cast<double*>(buf_info.ptr);
        size_t num_elements = buf_info.size;
        // Perform operations on the NumPy array data
        // ...
    }
}

PYBIND11_MODULE(my_module, m) {
    m.def("process_numpy_arrays", &process_numpy_arrays, "A function that processes a list of NumPy arrays.");
}

#if 0
import numpy as np
import my_module # Assuming my_module.cpp is compiled and available

# Create a list of NumPy arrays
list_of_arrays = [
    np.array([1.0, 2.0, 3.0], dtype=np.double),
    np.array([[4.0, 5.0], [6.0, 7.0]], dtype=np.double)
]

# Pass the list to the C++ function
my_module.process_numpy_arrays(list_of_arrays)
#endif
