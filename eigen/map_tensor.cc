/*
c++ -o /tmp/test test.cc -I/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3 -std=c++17 -g
 */
#include <iostream>
#include <vector>
#include <unsupported/Eigen/CXX11/Tensor>

int main() {
    // Raw data in row-major order
    float raw_data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int rows = 3;
    int cols = 4;

    // Create an Eigen::TensorMap with RowMajor layout
    Eigen::TensorMap<Eigen::Tensor<float, 2, Eigen::RowMajor>> my_tensor_map(raw_data, rows, cols);

    // Access elements and perform operations
    std::cout << "TensorMap (RowMajor):\n" << my_tensor_map << std::endl;

    // Modify data through the TensorMap
    my_tensor_map(0, 0) = 100.0f;
    std::cout << "\nModified TensorMap:\n" << my_tensor_map << std::endl;

    // Verify that the underlying raw data is also modified
    std::cout << "\nModified raw_data[0]: " << raw_data[0] << std::endl;

    return 0;
}
