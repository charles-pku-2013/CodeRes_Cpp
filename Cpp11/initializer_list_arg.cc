#include <iostream>
#include <initializer_list>

// Function that takes an std::initializer_list
void print_list(std::initializer_list<int> list) {
    std::cout << "Received " << list.size() << " elements:" << std::endl;
    for (auto& value : list) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

template<typename List>
void print_list_template(List&& list) {
    std::cout << "Received " << list.size() << " elements:" << std::endl;
    for (auto& value : list) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Call the function with a brace-enclosed list of values
    print_list({10, 20, 30, 40, 50});
    print_list_template<std::initializer_list<int>>({10, 20, 30, 40, 50});  // 必须显示指定模板参数
    return 0;
}


template <typename T, typename Shape>
py::array_t<T> build_py_array(const Shape& shape) {
    std::size_t len = std::accumulate(shape.begin(), shape.end(), 1,
                                      std::multiplies<typename Shape::value_type>());

    T*   data = new T[len]{0};
    auto capsule = py::capsule(data, free_data<T>);
    // DLOG(INFO) << fmt::format("Allocating {} memory, address={:x}", len, (uint64_t)data);
    return py::array_t<T>(shape, data, capsule);
}

auto& shape = snap_->outputs[i].shape;
auto py_arr = build_py_array<
    float, std::initializer_list<std::remove_reference_t<decltype(shape)>::value_type>>(
    {1, shape[1], shape[2], shape[3]});
