/*
c++ -o /tmp/test test.cc -I/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3 -std=c++17 -g
 */
#include <Eigen/Dense>
#include <iostream>
#include <algorithm>
#include <iterator>

int main() {
    // Eigen::MatrixXf mat(2, 2);
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> mat(2, 2);

    std::cout << mat.size() << std::endl;

    mat << 1.0, 2.0,
           3.0, 4.0;

    std::cout << mat << std::endl;

    // float* data_ptr_matrix = mat.data();
    std::copy(mat.data(), mat.data() + mat.size(), std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}
