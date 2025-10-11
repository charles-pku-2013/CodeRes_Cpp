#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>

namespace py = pybind11;

// Function to convert numpy array to cv::Mat
cv::Mat convert_py_array_to_cv_mat(py::array &img /* already cv::imdecode */) {
    py::buffer_info buf = img.request();
    // Use the OpenCV C++ API to create a cv::Mat from the numpy array's data
    if (buf.ndim == 3) {
        return cv::Mat(buf.shape[0], buf.shape[1], CV_8UC3, buf.ptr);
    } else if (buf.ndim == 2) {
        return cv::Mat(buf.shape[0], buf.shape[1], CV_8UC1, buf.ptr);
    } else {
        throw std::runtime_error("Unsupported number of dimensions.");
    }
}

// Example C++ function that takes a cv::Mat
void process_image_in_cpp(cv::Mat &input_mat) {
    std::cout << "C++ received a matrix of size: " << input_mat.rows << "x" << input_mat.cols << std::endl;
    // Perform some OpenCV operation on the C++ side, e.g., converting to grayscale
    cv::Mat gray_mat;
    cv::cvtColor(input_mat, gray_mat, cv::COLOR_BGR2GRAY);
    std::cout << "Image processed in C++ and converted to grayscale." << std::endl;
}

// pybind11 module definition
PYBIND11_MODULE(my_cpp_module, m) {
    m.doc() = "pybind11 example plugin";

    // Bind the C++ function to Python
    m.def("process_image", [](py::array img) {
        // Convert the numpy array to cv::Mat
        cv::Mat mat_from_python = convert_py_array_to_cv_mat(img);
        // Call the C++ processing function
        process_image_in_cpp(mat_from_python);
    }, "A function that processes a numpy array (image) in C++.");
}


#if 0
import cv2
import numpy as np
import my_cpp_module

# Create a sample numpy array (representing an image)
img_python = np.zeros((100, 100, 3), dtype=np.uint8)

# Call the C++ function and pass the numpy array
my_cpp_module.process_image(img_python)
#endif
