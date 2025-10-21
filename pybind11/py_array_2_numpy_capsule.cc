#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <vector>

namespace py = pybind11;

// Destructor function for the py::capsule
void free_data(void* data) {
    delete[] static_cast<double*>(data);
}

py::array_t<double> create_owned_array() {
    // 1. Allocate memory in C++
    size_t size = 10;
    double* data = new double[size];
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<double>(i * 2.0);
    }

    // 2. Create a py::capsule to manage the memory
    // The free_data function will be called when the capsule is destroyed.
    auto capsule = py::capsule(data, free_data);

    // 3. Construct py::array_t with the capsule
    // The last argument (capsule) transfers ownership to Python.
    return py::array_t<double>(
        {size},        // Shape
        {sizeof(double)}, // Strides
        data,          // Data pointer
        capsule        // Ownership capsule
    );
}

// another example
py::array_t<double> create_numpy_array_from_vector(size_t size) {
    // Create a C++ vector.
    auto v = std::vector<double>(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = static_cast<double>(i * 0.5);
    }

    // Create a capsule to manage the lifetime of the C++ vector.
    // The capsule takes ownership of the vector via std::move.
    py::capsule free_vec(v.data(), [v = std::move(v)](void *) {});

    // Construct the NumPy array, passing the capsule to manage its memory.
    // The last argument is the "base" object that provides memory ownership.
    return py::array_t<double>(
        size,           // Shape
        v.data(),       // C++ data pointer
        free_vec);      // Capsule holding the vector
}

PYBIND11_MODULE(example, m) {
    m.def("create_numpy_array_from_vector", &create_numpy_array_from_vector);
}

PYBIND11_MODULE(my_module, m) {
    m.def("create_owned_array", &create_owned_array, "Create a NumPy array owned by Python");
}
