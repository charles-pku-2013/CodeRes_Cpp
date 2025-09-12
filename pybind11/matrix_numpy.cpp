/*
https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++17 -undefined dynamic_lookup $(python3 -m pybind11 --includes) -I/opt/homebrew/Cellar/fmt/11.2.0/include -L/opt/homebrew/Cellar/fmt/11.2.0/lib -lfmt matrix.cpp -o example$(python3-config --extension-suffix)
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <fmt/base.h>
#include <fmt/printf.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>
#include <iostream>

namespace py = pybind11;

class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols) : m_rows(rows), m_cols(cols) {
        std::cerr << fmt::sprintf("Matrix construct %lx", (uint64_t)(this)) << std::endl;
        m_data = new float[(size_t) (rows * cols)];
        memset(m_data, 0, sizeof(float) * (size_t) (rows * cols));
    }

    Matrix(const Matrix &s) : m_rows(s.m_rows), m_cols(s.m_cols) {
        std::cerr << fmt::sprintf("Matrix copy construct %lx", (uint64_t)(this)) << std::endl;
        m_data = new float[(size_t) (m_rows * m_cols)];
        memcpy(m_data, s.m_data, sizeof(float) * (size_t) (m_rows * m_cols));
    }

    Matrix(Matrix &&s) noexcept : m_rows(s.m_rows), m_cols(s.m_cols), m_data(s.m_data) {
        std::cerr << fmt::sprintf("Matrix move construct %lx", (uint64_t)(this)) << std::endl;
        s.m_rows = 0;
        s.m_cols = 0;
        s.m_data = nullptr;
    }

    ~Matrix() {
        std::cerr << fmt::sprintf("Matrix destruct %lx", (uint64_t)(this)) << std::endl;
        if (m_data) {
            delete [] m_data;
        }
    }

    Matrix &operator=(const Matrix &s) {
        if (this == &s) {
            return *this;
        }
        delete[] m_data;
        m_rows = s.m_rows;
        m_cols = s.m_cols;
        m_data = new float[(size_t) (m_rows * m_cols)];
        memcpy(m_data, s.m_data, sizeof(float) * (size_t) (m_rows * m_cols));
        return *this;
    }

    Matrix &operator=(Matrix &&s) noexcept {
        if (&s != this) {
            delete[] m_data;
            m_rows = s.m_rows;
            m_cols = s.m_cols;
            m_data = s.m_data;
            s.m_rows = 0;
            s.m_cols = 0;
            s.m_data = nullptr;
        }
        return *this;
    }

    float operator()(std::size_t i, std::size_t j) const {
        return m_data[(size_t) (i * m_cols + j)];
    }

    float &operator()(std::size_t i, std::size_t j) {
        return m_data[(size_t) (i * m_cols + j)];
    }

    std::string to_string() const {
        std::string result;

        for (auto i = 0; i < m_rows; ++i) {
            auto* arr = m_data + i * m_cols;
            result.append(fmt::sprintf("[%s]\n", fmt::to_string(fmt::join(arr, arr + m_cols, ", "))));
        }

        return result;
    }

    float *data() { return m_data; }

    std::size_t rows() const { return m_rows; }
    std::size_t cols() const { return m_cols; }

private:
    std::size_t m_rows;
    std::size_t m_cols;
    float *m_data = nullptr;
};


PYBIND11_MODULE(example, m) {
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<py::ssize_t, py::ssize_t>())
        /// Construct from a buffer
        .def(py::init([](const py::buffer &b) {
            py::buffer_info info = b.request();

            fmt::println(std::cerr, "buffer_info: itemsize={}, size={}, format={}, ndim={}, shape={}, strides={}, readonly={}",
                    info.itemsize, info.size, info.format, info.ndim,
                    fmt::to_string(info.shape), fmt::to_string(info.strides),
                    info.readonly);

            if (info.format != py::format_descriptor<float>::format() || info.ndim != 2) {
                throw std::runtime_error("Incompatible buffer format!");
            }
            // 当前只支持二维数组
            auto *v = new Matrix(info.shape[0], info.shape[1]);
            memcpy(v->data(), info.ptr, sizeof(float) * (size_t) (v->rows() * v->cols()));
            return v;
        }))

        .def("rows", &Matrix::rows)
        .def("cols", &Matrix::cols)
        .def("__repr__", &Matrix::to_string)

        /// Bare bones interface
        .def("__getitem__",
             [](const Matrix &m, std::pair<py::ssize_t, py::ssize_t> i) {
                 if (i.first >= m.rows() || i.second >= m.cols()) {
                     throw py::index_error();
                 }
                 return m(i.first, i.second);
             })
        .def("__setitem__",
             [](Matrix &m, std::pair<py::ssize_t, py::ssize_t> i, float v) {
                 if (i.first >= m.rows() || i.second >= m.cols()) {
                     throw py::index_error();
                 }
                 m(i.first, i.second) = v;
             })
        /// Provide buffer access  由Matrix构建buffer_info
        .def_buffer([](Matrix &m) -> py::buffer_info {
            return py::buffer_info(
                m.data(),                          /* Pointer to buffer */
                // sizeof(float),                          [> Size of one scalar <]
                // py::format_descriptor<float>::format(), [> Python struct-style format descriptor <]
                // 2,                                      [> Number of dimensions <]
                {m.rows(), m.cols()},              /* Buffer dimensions */
                {sizeof(float) * size_t(m.cols()), /* Strides (in bytes) for each index */
                 sizeof(float)});
        });
}

#if 0 // test.py
from example import *
import numpy as np
# Matrix to NumPy
m = Matrix(2,3)
print(m)
a = np.array(m, copy=False)
print(a)
m[1, 1] = 5.0
print(a)   # 可以看到修改
# NumPy to Matrix
m1 = Matrix(a)
print(m1)

# NumPy to Matrix
a = np.array([[1,2,3],[4,5,6]], dtype="float32")
m = Matrix(a)
print(m)
a[1,1] = 100
print(m)  # a的修改不能反馈给m
#endif

// py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
//    .def_buffer([](Matrix &m) -> py::buffer_info {
//         return py::buffer_info(
//             m.data(),                               /* Pointer to buffer */
//             sizeof(float),                          /* Size of one scalar */
//             py::format_descriptor<float>::format(), /* Python struct-style format descriptor */
//             2,                                      /* Number of dimensions */
//             { m.rows(), m.cols() },                 /* Buffer dimensions */
//             { sizeof(float) * m.cols(),             /* Strides (in bytes) for each index */
//               sizeof(float) }
//         );
//     });
