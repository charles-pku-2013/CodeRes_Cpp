/*
c++ -o /tmp/test test.cc -I/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3 -std=c++17 -g
 */
#include <Eigen/Dense>
#include <iostream>
#include <algorithm>
#include <iterator>

int main() {
    using Matrix = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
    // Eigen::MatrixXf mat(2, 2);
    // Matrix mat(2, 2);
    Matrix mat;

    std::cout << mat << std::endl;
    std::cout << mat.size() << std::endl;
    std::cout << (uint64_t)(mat.data()) << std::endl;

    mat.conservativeResize(2, 2);

    mat << 1.0, 2.0,
           3.0, 4.0;

    std::cout << mat << std::endl;
    std::cout << (uint64_t)(mat.data()) << std::endl;

    // mat.conservativeResize(1, 1);  // column change will result in addr change
    mat.conservativeResize(1, 2);
    std::cout << (uint64_t)(mat.data()) << std::endl;
    mat.conservativeResize(2, 2);
    std::cout << (uint64_t)(mat.data()) << std::endl;
    mat.conservativeResize(3, 2);   // row increase will lead to addr change
    std::cout << (uint64_t)(mat.data()) << std::endl;
    std::cout << mat << std::endl << std::endl;;

    // mat.setZero();
    // std::cout << mat << std::endl;

    mat = Matrix(3, 2);
    std::cout << mat << std::endl;
    std::cout << (uint64_t)(mat.data()) << std::endl;

    // mat.conservativeResize(1, 2);

    // std::cout << "After resize: " << mat << std::endl;
    // std::cout << (uint64_t)(mat.data()) << std::endl;  // addr same

    // Get the first 3 rows
    // Eigen::MatrixXd first_n_rows = mat.topRows(2);

    // float* data_ptr_matrix = mat.data();
    // std::copy(mat.data(), mat.data() + mat.size(), std::ostream_iterator<float>(std::cout, " "));
    // std::cout << std::endl;

    return 0;
}

// submatrix
// int rows = 2;
// int cols = 2;
// int startRow = 1;
// int startCol = 1;
// Eigen::MatrixXd subMatrix = originalMatrix.block(startRow, startCol, rows, cols); // 2x2 block starting at (1,1)
