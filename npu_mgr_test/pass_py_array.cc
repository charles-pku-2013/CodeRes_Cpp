// parent
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <pybind11/embed.h> // 包含 pybind11 头文件
#include <pybind11/numpy.h>
#include <cstdint>
#include <cstring>

namespace py = pybind11;

// 辅助函数，用于将数据写入管道
void write_to_pipe(int pipe_fd, const void* data, size_t size) {
    if (write(pipe_fd, data, size) != (ssize_t)size) {
        perror("write to pipe failed");
        exit(EXIT_FAILURE);
    }
}

void parent_process(int pipe_write_fd, const std::vector<py::array_t<uint8_t>>& data_vec) {
    // 1. 发送向量的大小
    size_t vec_size = data_vec.size();
    write_to_pipe(pipe_write_fd, &vec_size, sizeof(vec_size));

    for (const auto& arr : data_vec) {
        // 2. 发送数组的维度
        int ndim = arr.ndim();
        write_to_pipe(pipe_write_fd, &ndim, sizeof(ndim));

        // 3. 发送数组的形状
        std::vector<ssize_t> shape;
        for (int i = 0; i < ndim; ++i) {
            shape.push_back(arr.shape()[i]);
        }
        write_to_pipe(pipe_write_fd, shape.data(), shape.size() * sizeof(ssize_t));

        // 4. 发送原始数据的大小
        size_t data_size = arr.nbytes();
        write_to_pipe(pipe_write_fd, &data_size, sizeof(data_size));

        // 5. 发送原始数据
        write_to_pipe(pipe_write_fd, arr.data(), data_size);
    }
    close(pipe_write_fd); // 关闭写端
    std::cout << "Parent: Data sent, pipe closed." << std::endl;
}

// child
// 辅助函数，用于从管道读取数据
void read_from_pipe(int pipe_fd, void* data, size_t size) {
    if (read(pipe_fd, data, size) != (ssize_t)size) {
        perror("read from pipe failed");
        exit(EXIT_FAILURE);
    }
}

std::vector<py::array_t<uint8_t>> child_process(int pipe_read_fd) {
    std::vector<py::array_t<uint8_t>> restored_vec;

    // 1. 读取向量的大小
    size_t vec_size;
    read_from_pipe(pipe_read_fd, &vec_size, sizeof(vec_size));

    for (size_t i = 0; i < vec_size; ++i) {
        // 2. 读取数组的维度
        int ndim;
        read_from_pipe(pipe_read_fd, &ndim, sizeof(ndim));

        // 3. 读取数组的形状
        std::vector<ssize_t> shape(ndim);
        read_from_pipe(pipe_read_fd, shape.data(), ndim * sizeof(ssize_t));

        // 4. 读取原始数据的大小
        size_t data_size;
        read_from_pipe(pipe_read_fd, &data_size, sizeof(data_size));

        // 5. 读取原始数据到一个临时的 std::vector<uint8_t>
        std::vector<uint8_t> raw_data(data_size);
        read_from_pipe(pipe_read_fd, raw_data.data(), data_size);

        // 6. 使用 pybind11 从数据中恢复 py::array_t
        // 注意: 使用 std::move 实现零拷贝，并将数据所有权转移给 py::array_t
        py::array_t<uint8_t> restored_arr(shape, raw_data.data()); // 先构造 array_t
        // 然后使用 capsule 将所有权转移，确保数据生命周期管理正确
        auto capsule = py::capsule(raw_data.data(), [](void *d) {
            delete reinterpret_cast<std::vector<uint8_t>*>(d);
        });
        // 实际上需要一个更精细的方法来让 py::array_t 接管 std::vector 的内存
        // 最简单的方法是复制数据（如果性能允许）
        // 或者使用 py::capsule (但这需要将 vector 包装到一个指针中，并在 capsule 中释放)

        // 简化起见，我们假设复制是可接受的，或者使用 pyarrow 等库
        // 对于这个例子，我们直接从已读取的 raw_data 构造（这是复制）
        // 如果要零拷贝，则需要使用更高级的内存管理技术或专门的 IPC 库 (如 Boost.Interprocess)

        // 这里的构造函数会复制数据
        py::array_t<uint8_t> final_arr(shape, raw_data.data()); 
        restored_vec.push_back(final_arr);
    }
    close(pipe_read_fd); // 关闭读端
    std::cout << "Child: Data received and restored, pipe closed." << std::endl;
    return restored_vec;
}
