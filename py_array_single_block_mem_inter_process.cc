template <typename T>
void free_data(void* data) {
    delete[] static_cast<T*>(data);
}

template <typename T, typename Shape>
py::array_t<T> build_py_array(const Shape& shape) {
    // 计算py_array_t所需内存长度
    std::size_t len = std::accumulate(shape.begin(), shape.end(), 1,
                                      std::multiplies<typename Shape::value_type>());

    T*   data = new T[len]{0};  // 分配py_array_t所需内存
    auto capsule = py::capsule(data, free_data<T>); // 指定py_array_t内存释放函数

    return py::array_t<T>(shape, data, capsule);
}

int main() {
    using ResultType = std::vector<std::vector<py::array_t<float>>>;

    ResultType results;
    results.resize(n_images_);
    for (auto& v : results) {
        v.reserve(n_outputs_);
        for (auto i = 0; i < n_outputs_; ++i) {
            auto& t_shape = snap_->outputs[i].shape;
            std::array<std::remove_reference_t<decltype(t_shape)>::value_type, 4> shape{
                1, t_shape[1], t_shape[2], t_shape[3]};
            auto py_arr = build_py_array<float, decltype(shape)>(shape);
            v.emplace_back(std::move(py_arr));
        }
    }
}

/*
 * 有这样一段使用pybind11的c++代码，现在想做如下修改：所有的py_array_t的内存都分配自一个单一连续的内存块，应该如何实现？
 */


/*
 * 好的，您希望将所有 py::array_t 的内存从多个单独的 new T[len] 分配，更改为分配自一个单一连续的内存块。这可以通过使用一个大型的 std::vector<T> 来管理这块单一内存，并让所有的 py::array_t 共享这个向量持有的数据指针和内存释放胶囊（capsule）来实现。
 * 实现思路
 * 管理内存的容器: 使用一个 std::vector<T> 来分配和管理所有数据所需的总内存。
 * 创建共享的 py::capsule: 只需要创建一个 py::capsule，它负责在所有 py::array_t 对象生命周期结束时释放这个 std::vector<T>。
 * 调整 build_py_array: 修改 build_py_array 函数，使其接受一个预先分配好的数据指针，并使用传入的共享 py::capsule。
 */

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <vector>
#include <numeric>
#include <functional>
#include <memory>

namespace py = pybind11;

// 辅助函数，计算给定形状的总长度
template <typename Shape>
std::size_t calculate_total_len(const Shape& shape) {
    return std::accumulate(shape.begin(), shape.end(), std::size_t(1),
                           std::multiplies<typename Shape::value_type>());
}

// 修改后的函数，使用外部提供的指针和胶囊
template <typename T, typename Shape>
py::array_t<T> build_py_array(const Shape& shape, T* data_ptr, const py::capsule& capsule) {
    // py::array_t 构造函数签名：{shape}, {strides}, data*, capsule
    // 我们传入预先计算好的形状、数据指针，并绑定同一个内存释放胶囊
    return py::array_t<T>(shape, data_ptr, capsule);
}

// 示例 main 函数（假设您在实际的 pybind11 模块绑定中使用）
void initialize_arrays_in_single_block(/* 其他必要的参数，例如 n_images_, snap_, n_outputs_ */) {
    using ResultType = std::vector<std::vector<py::array_t<float>>>;

    // 1. 计算所有 py::array_t 所需的总内存大小
    std::size_t total_memory_len = 0;

    // 假设这些参数可用
    int n_images_ = 2; // 示例值
    int n_outputs_ = 3; // 示例值
    // 假设 snap_ 对象和其 outputs 结构是有效的，这里用一个模拟结构
    struct OutputInfo { std::array<int, 4> shape; };
    std::vector<OutputInfo> outputs_info(n_outputs_);
    // 模拟形状数据（例如 NCHW 中的 CHW）
    outputs_info[0].shape = {1, 64, 32, 32};
    outputs_info[1].shape = {1, 128, 16, 16};
    outputs_info[2].shape = {1, 256, 8, 8};

    for (auto img_idx = 0; img_idx < n_images_; ++img_idx) {
        for (auto i = 0; i < n_outputs_; ++i) {
            auto& t_shape = outputs_info[i].shape;
            // 使用上面定义的辅助函数计算长度
            total_memory_len += calculate_total_len(t_shape);
        }
    }

    // 2. 分配单一连续的内存块，并用智能指针管理生命周期
    // 使用 std::vector 来管理这块内存是最简单且推荐的方式。
    // 我们使用 std::unique_ptr 来持有这个向量，以便将其所有权转移到 py::capsule 中。
    auto full_data_vector_ptr = std::make_unique<std::vector<float>>(total_memory_len, 0.0f);
    float* base_ptr = full_data_vector_ptr->data();

    // 3. 创建一个 py::capsule，当它被销毁时，它会释放整个向量
    // pybind11 胶囊可以接受一个 lambda 函数作为析构函数。
    auto capsule = py::capsule(full_data_vector_ptr.release(), [](void* raw_ptr) {
        // 析构函数负责删除那个 std::vector 指针
        delete static_cast<std::vector<float>*>(raw_ptr);
    });

    // 4. 填充 results 结构体，所有 py::array_t 共享同一个 capsule
    ResultType results;
    results.resize(n_images_);
    float* current_ptr = base_ptr; // 跟踪当前内存偏移量

    for (auto& v : results) {
        v.reserve(n_outputs_);
        for (auto i = 0; i < n_outputs_; ++i) {
            auto& t_shape = outputs_info[i].shape;
            // 确保形状是 std::array，符合原始代码意图
            std::array<int, 4> shape{t_shape[0], t_shape[1], t_shape[2], t_shape[3]};

            // 使用修改后的 build_py_array，传入当前的指针偏移量和共享的 capsule
            auto py_arr = build_py_array<float, decltype(shape)>(shape, current_ptr, capsule);
            v.emplace_back(std::move(py_arr));

            // 更新指针偏移量，指向下一个数组的起始位置
            current_ptr += calculate_total_len(shape);
        }
    }

    // 现在所有的 py::array_t 都创建完毕，并且都引用同一块内存 (base_ptr)，
    // 并且它们都共享同一个 capsule。只要任何一个 py::array_t 或 capsule 存活，内存就不会被释放。
    // 当 C++ 的 result 向量被返回给 Python 或超出作用域时，
    // Python 垃圾回收机制会管理这些 py::array_t 对象及其共享的 capsule。
}

/*
 * 关键点解释：
 * 单一所有权: 我们使用 std::unique_ptr<std::vector<float>> 来管理原始的内存块，并将其所有权转移给 py::capsule。
 * 共享 py::capsule: 所有的 py::array_t 实例在构造时都传入同一个 py::capsule 对象。pybind11 会自动处理引用计数：只有当最后一个引用该 capsule 的对象被销毁时（即所有 py::array_t 都被垃圾回收后），capsule 的析构函数（我们的 lambda）才会被调用，进而释放整个 std::vector。
 * 指针偏移: 在循环中，我们使用一个 current_ptr 来跟踪我们正在向哪个内存地址分配下一个 py::array_t 的数据。
 * 通过这种方式，您成功地将内存分配集中在一个调用中，并确保了 Python 端的内存安全管理。
 */

/*
 * 现在想用共享内存技术如boost::interprocess让其他进程能直接访问这个共享内存块，并且从这个内存块中复原出ResultType，该如何实现？
 */


/*
 * 1. 定义共享内存友好的数据结构（元数据）
 * 在头文件中定义一个跨进程共享的结构体。
 */
// common_types.h (需要在所有进程中包含)

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <array>
#include <vector>

// 使用 Boost 的分配器，该分配器知道如何在共享内存中分配对象
template <typename T>
using ShmemAllocator = boost::interprocess::allocator<T, boost::interprocess::managed_shared_memory::segment_manager>;

// 存储单个数组的布局信息
struct ArrayLayout {
    std::array<int, 4> shape;
    std::size_t byte_offset; // 从数据块开始的字节偏移量
    std::size_t byte_length; // 数据的总字节长度
};

// 存储所有数组布局的向量，使用共享内存分配器
using LayoutVector = std::vector<ArrayLayout, ShmemAllocator<ArrayLayout>>;


/*
 * 2. 进程 A (数据生产者 & 共享内存创建者)
 * 此进程负责创建共享内存并写入数据。
 */
// process_a.cpp

#include "common_types.h"
#include <iostream>
#include <numeric>
#include <cstring> // For std::memcpy

namespace bip = boost::interprocess;

void create_shared_data(/* ... 原始参数 ... */) {
    const char* shmem_name = "MyImageResultsShmem";
    const std::size_t shmem_size = 100 * 1024 * 1024; // 示例大小：100 MB

    // 移除旧的共享内存段（如果存在）
    bip::shared_memory_object::remove(shmem_name);

    // 创建托管的共享内存段
    bip::managed_shared_memory segment(bip::create_only, shmem_name, shmem_size);

    // 使用 segment 的管理器为 LayoutVector 分配器构造一个实例
    ShmemAllocator<ArrayLayout> alloc_inst(segment.get_segment_manager());

    // 在共享内存中构造 LayoutVector
    LayoutVector* layout_vec = segment.construct<LayoutVector>("LayoutVector")(alloc_inst);

    // --- 写入数据阶段 ---
    // 假设您已经计算出了 total_memory_len (总浮点数个数)
    std::size_t total_float_len = /* ... from previous logic ... */;
    std::size_t total_data_bytes = total_float_len * sizeof(float);

    // 在共享内存中分配实际的原始浮点数据块
    float* shared_float_data = segment.alloc<float>(total_float_len);

    std::size_t current_byte_offset = 0;

    // 假设您有原始的 C++ 数据来源 results_source
    // 遍历原始数据，写入共享内存，并填充 layout_vec
    for (const auto& image_vec : results_source) {
        for (const auto& py_arr : image_vec) {
            ArrayLayout layout;
            // 填充形状信息
            auto shape = py_arr.shape(); // 从 py::array_t 获取形状
            for(size_t i=0; i<4; ++i) layout.shape[i] = shape[i];

            std::size_t len = calculate_total_len(layout.shape); // 辅助函数计算长度
            layout.byte_offset = current_byte_offset;
            layout.byte_length = len * sizeof(float);

            // 将数据从本地内存复制到共享内存
            std::memcpy(
                reinterpret_cast<char*>(shared_float_data) + layout.byte_offset,
                py_arr.data(), // 获取本地 py::array_t 的数据指针
                layout.byte_length
            );

            layout_vec->push_back(layout);
            current_byte_offset += layout.byte_length;
        }
    }
    
    std::cout << "Data written to shared memory. Producers job done." << std::endl;
    // main 函数结束时，shared_memory_object 会保持打开状态，直到明确移除。
}

// process_b.cpp

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <vector>
#include <numeric>
#include <functional>
#include <array>
#include <stdexcept>

namespace py = pybind11;
namespace bip = boost::interprocess;

// 辅助函数（假设在 A 和 B 中共享）
template <typename Shape>
std::size_t calculate_total_len(const Shape& shape) {
    return std::accumulate(shape.begin(), shape.end(), std::size_t(1),
                           std::multiplies<typename Shape::value_type>());
}

// 模拟您的全局配置参数，进程 A 和 B 必须一致
const int n_images_ = 2; 
const int n_outputs_ = 3;
// 模拟 snap_->outputs 结构体，形状信息需要一致
const std::vector<std::array<int, 4>> OUTPUT_SHAPES = {
    {1, 64, 32, 32},
    {1, 128, 16, 16},
    {1, 256, 8, 8}
};

/*
 * 消费者进程 B 需要做以下几件事：
 * 连接共享内存: 使用 boost::interprocess 连接到生产者 A 创建的共享内存段。
 * 获取基指针: 获得共享内存中实际浮点数数据块的基地址（float*）。
 * 重建本地结构: 遍历已知的 n_images_ 和 n_outputs_ 维度，根据预定的形状和顺序，计算出每个子数组在共享内存中的准确地址。
 * 构建 py::array_t: 使用这些计算出的指针和形状，在进程 B 中构造 pybind11::array_t 对象。
 * 关键点：管理 py::array_t 的生命周期
 * 在构建 py::array_t 时，您必须非常小心地管理内存所有权。原始代码使用 py::capsule 来释放内存。但在进程 B 中，我们不能释放这块内存，因为它属于 Boost 管理的共享内存段，并且可能在进程 B 结束后仍然需要存在（或者由进程 A 负责管理生命周期）。
 * 因此，我们需要创建一个特殊的 py::capsule，它什么也不做（一个空操作析构函数），或者确保 py::array_t 的构造函数以只读模式使用，并且不附加析构器。
 */
// 消费者进程 B 的恢复函数
std::vector<std::vector<py::array_t<float>>> recover_result_type_from_shmem() {
    using ResultType = std::vector<std::vector<py::array_t<float>>>;
    const char* shmem_name = "MyImageResultsShmem";

    // 1. 连接到已有的共享内存段
    bip::managed_shared_memory segment(bip::open_only, shmem_name);

    // 2. 找到生产者 A 写入的原始数据块的基地址
    // 注意：查找名称需要与 A 中分配时使用的名称匹配。
    // 在 A 中我们使用了 segment.alloc<float>，它没有指定名称，所以我们需要找到整个段的基指针
    // （或者在 A 中使用 segment.construct<float[]>("FloatData") 来命名数据块）
    
    // 假设 A 在创建时将数据命名为 "FloatData"
    float* base_ptr = segment.find<float>("FloatData").first;

    if (!base_ptr) {
        throw std::runtime_error("Could not find 'FloatData' in shared memory segment.");
    }

    // 3. 重建本地 ResultType 结构，并使用偏移量构建 py::array_t
    ResultType results;
    results.resize(n_images_);
    float* current_ptr = base_ptr; // 跟踪当前在共享内存中的指针位置

    // !!! 核心区别 !!!
    // 创建一个空的 py::capsule。当 py::array_t 被销毁时，这个胶囊什么都不会做，
    // 从而避免了双重释放或错误释放共享内存。
    // 我们只是告诉 pybind11 "这里有数据，但你不需要管释放它"。
    py::capsule no_release_capsule(); 
    // 或者更明确地，使用一个空的析构函数lambda：
    auto shared_mem_capsule = py::capsule(nullptr, [](void*){ /* Do nothing on destruction */ });


    for (auto& v : results) {
        v.reserve(n_outputs_);
        for (auto i = 0; i < n_outputs_; ++i) {
            auto& t_shape = OUTPUT_SHAPES[i];
            
            // 构造符合 py::array_t 期望的形状数组
            std::array<int, 4> shape_arr{t_shape[0], t_shape[1], t_shape[2], t_shape[3]};
            
            // 计算当前数组的长度，用于指针偏移
            std::size_t len = calculate_total_len(shape_arr);

            // 使用 build_py_array 变体，传入当前指针和空的 capsule
            // py::array_t 构造函数签名：{shape}, {strides}, data*, capsule
            auto py_arr = py::array_t<float>(shape_arr, current_ptr, shared_mem_capsule);
            
            v.emplace_back(std::move(py_arr));

            // 更新指针到下一个数组的起始位置
            current_ptr += len;
        }
    }
    
    // 返回构建好的 ResultType
    return results;
}
