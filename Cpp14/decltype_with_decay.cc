using Result = std::vector<std::vector<py::array_t<float>>>;

/**
 * @brief  py::array_t 数据释放函数，python里numpy对象销毁时调用
 */
template <typename T>
void free_data(void* data) {
    DLOG(INFO) << fmt::sprintf("Freeing result data %lx", (uint64_t)data);
    delete[] static_cast<T*>(data);
}

template <typename T, typename Shape>
py::array_t<T> build_py_array(const Shape& shape) {
    std::size_t len = std::accumulate(shape.begin(), shape.end(), 1,
                        std::multiplies<typename Shape::value_type>());

    T* data = new T[len]{0};  // NOTE 初始化数组全部元素 <=> memset
    auto  capsule = py::capsule(data, free_data<T>);

    DLOG(INFO) << fmt::format("Allocating {} memory, address={:x}", len, (uint64_t)data);

    return py::array_t<T>(shape, data, capsule);
}

Result test(const std::vector<uint32_t>& shape) {
    Result result;

    result.resize(1);
    for (auto i = 0; i < 1; ++i) {
        result[i].resize(1);
        for (auto j = 0; j < 1; ++j) {
            result[i][j] = build_py_array<float, std::decay_t<decltype(shape)>>(shape);
        }
    }

    return result;
}
