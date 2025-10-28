#include <iostream>
#include <vector>
#include <numeric> // For std::iota

// A function to split a vector into N equal parts
template <typename T>
std::vector<std::vector<T>> split_vector(const std::vector<T>& vec, size_t num_parts) {
    if (num_parts == 0) {
        return {};
    }
    if (vec.empty()) {
        return std::vector<std::vector<T>>(num_parts, std::vector<T>());
    }

    std::vector<std::vector<T>> result;
    size_t vec_size = vec.size();
    size_t base_size = vec_size / num_parts;
    size_t remainder = vec_size % num_parts;

    auto current_it = vec.begin();

    for (size_t i = 0; i < num_parts; ++i) {
        size_t part_size = base_size + (i < remainder ? 1 : 0);
        result.emplace_back(current_it, current_it + part_size);
        current_it += part_size;
    }
    return result;
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // std::vector<int> numbers = {1};
    // std::vector<int> numbers;

    // Split the vector into 3 parts
    // size_t num_parts = 3;
    size_t num_parts = 1;
    std::vector<std::vector<int>> split_parts = split_vector(numbers, num_parts);

    // Print the result
    for (size_t i = 0; i < split_parts.size(); ++i) {
        std::cout << "Part " << i + 1 << ": ";
        for (int val : split_parts[i]) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
