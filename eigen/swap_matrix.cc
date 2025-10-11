#include <Eigen/Dense>
#include <iostream>

int main() {
    Eigen::MatrixXd mat1(2, 2);
    mat1 << 1, 2,
            3, 4;

    Eigen::MatrixXd mat2(2, 2);
    mat2 << 5, 6,
            7, 8;

    std::cout << "Before swap:" << std::endl;
    std::cout << "mat1:\n" << mat1 << std::endl;
    std::cout << "mat2:\n" << mat2 << std::endl;

    mat1.swap(mat2); // Swaps the contents of mat1 and mat2

    std::cout << "\nAfter swap:" << std::endl;
    std::cout << "mat1:\n" << mat1 << std::endl;
    std::cout << "mat2:\n" << mat2 << std::endl;

    return 0;
}