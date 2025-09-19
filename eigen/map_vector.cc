/*
c++ -o /tmp/test test.cc -I/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3 -std=c++17 -g
 */
#include <Eigen/Dense>
#include <vector>
#include <iostream>

template <typename MatrixType, typename PtrType = typename MatrixType::Scalar*>
std::vector<PtrType>
matrix_to_ptr_array(MatrixType& mat, std::size_t start = 0, std::size_t end = static_cast<std::size_t>(-1)) {
    end = std::min(static_cast<std::size_t>(mat.rows()), end);
    assert(start < end);

    std::vector<PtrType> result;
    result.reserve(end - start);

    for (auto i = start; i < end; ++i) {
        result.emplace_back(static_cast<PtrType>(mat.row(i).data()));
    }

    return result;
}

int main() {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    // Map the std::vector data to an Eigen::MatrixXd
    // The arguments are: data pointer, number of rows, number of columns
    using Mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
    Eigen::Map<Mat> mat(data.data(), 2, 3);
    // Eigen::Map<Eigen::MatrixXd> mat(data.data(), 2, 3);  // col major
    std::cout << "Eigen Matrix from std::vector:\n" << mat << std::endl;

    std::cout << "Row 1" << std::endl;
    auto* ptr = mat.row(1).data();
    std::copy(ptr, ptr + mat.row(1).size(), std::ostream_iterator<double>(std::cout, " "));
    std::cout << std::endl << std::endl;

    data[2] = 10.0;
    std::cout << "After modify vector:\n" << mat << std::endl;

    mat(1, 1) = 20.0;
    std::cout << "After modify matrix:\n";
    std::copy(data.begin(), data.end(), std::ostream_iterator<double>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}
