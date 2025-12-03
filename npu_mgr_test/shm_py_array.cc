/*
 * 以下代码假设您在需要写入数据的进程中调用此逻辑，并且该进程负责创建共享内存。
 */
#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <fmt/core.h> // 假设您使用 fmt 库进行日志记录

namespace py = pybind11;
namespace bip = boost::interprocess;

// 定义共享内存段的名称
const char* SHARED_MEMORY_NAME = "MySharedMemorySegment";
// 定义共享内存的预估大小（需要根据您的实际需求调整）
const std::size_t SHARED_MEMORY_SIZE = 65536000; // 比如 64MB

using ResultType = std::vector<std::vector<py::array_t<float>>>;

// 假设这些变量在您的原始类作用域内可用
// extern size_t n_outputs_; 
// extern SomeStruct* snap_; // 包含 output shape 信息

ResultType init_results_shared_memory(std::size_t n_imgs, std::size_t n_outputs_, SomeStruct* snap_) {
    
    // 1. 计算所有 py::array_t 所需的总内存大小 (与原代码相同)
    auto get_py_array_len = [](const auto& shape) -> std::size_t {
        return std::accumulate(shape.begin(), shape.end(), std::size_t(1),
                               std::multiplies<typename std::remove_reference_t<decltype(shape)>::value_type>());
    };

    std::size_t total_len = 0;
    std::vector<std::size_t> output_sizes(n_outputs_);
    for (auto i = 0; i < n_outputs_; ++i) {
        auto& t_shape = snap_->outputs[i].shape;
        // 使用一个通用的 shape 结构，假设您的 snap_ 结构体兼容
        std::array<int, 4> shape{ 1, t_shape[1], t_shape[2], t_shape[3] };
        std::size_t arr_len = get_py_array_len(shape);
        total_len += arr_len;
        output_sizes[i] = arr_len;
    }
    total_len *= n_imgs;
    std::size_t total_bytes = total_len * sizeof(float);

    // --- 跨进程修改开始 ---

    // 2. 创建或打开一个 boost 托管共享内存段
    // 使用 managed_shared_memory 替换原始的 new[] 分配
    
    // 我们在这里创建（或打开已有的）共享内存。 
    // 注意：在实际的多进程场景中，一个进程需要负责创建，另一个进程负责打开。
    // 为了简化，我们假设这个函数是在负责创建和写入数据的那个进程中调用的。
    
    // 如果需要一个更健壮的创建/打开逻辑，可以使用 try/catch 切换 create_only 或 open_only
    bip::managed_shared_memory segment(bip::create_only, SHARED_MEMORY_NAME, total_bytes + 1024); // 预留一些额外空间

    // 3. 在共享内存中分配原始数据块
    float* p_result_data = segment.allocate<float>(total_len);
    // 初始化内存为 0.0f
    std::fill_n(p_result_data, total_len, 0.0f); 

    DLOG(INFO) << fmt::format("Allocated {} bytes memory in shared segment '{}', addr={:x}", 
                              total_bytes, SHARED_MEMORY_NAME, (uint64_t)p_result_data);

    // 4. 创建一个 py::capsule，管理共享内存的生命周期
    // Pybind11 的 capsule 析构函数必须是通用的 C 风格指针回调。
    // 释放共享内存的方式与释放堆内存不同，通常我们依赖于创建共享内存的进程退出，或者手动调用 bip::shared_memory_object::remove(SHARED_MEMORY_NAME)。
    
    // 当 py::array_t 在 Python 端被垃圾回收时，这个 capsule 会触发，但它不能直接删除共享内存段本身（段通常由进程生命周期管理）。
    // 这个 capsule 的作用主要是记录日志。
    auto capsule = py::capsule(p_result_data, [](void* ptr) {
        // 注意：我们通常不会在这里释放 boost shared memory 的整个 segment，
        // 我们只是记录这个特定指针的生命周期。
        DLOG(INFO) << fmt::format("Pybind11 capsule finished for shared data pointer {:x}. "
                                  "Shared Memory segment is persistent until explicitly removed or process exits.", 
                                  (uint64_t)ptr);
        // 如果需要，可以在这里手动 remove 共享内存对象，但这通常在主程序退出时完成
        // bip::shared_memory_object::remove(SHARED_MEMORY_NAME); 
    });

    // 5. 使用 build_py_array 助手函数构建 py::array_t 对象 (与原代码相同)
    auto build_py_array = [](const auto& shape, auto* data_ptr, const py::capsule& capsule) {
        using T = typename std::remove_pointer_t<decltype(data_ptr)>;
        // 关键点：将指向共享内存的指针传递给 py::array_t
        return py::array_t<T>(shape, data_ptr, capsule); 
    };

    ResultType results(n_imgs);
    float* current_ptr = p_result_data; // 跟踪当前内存偏移量
    for (auto& v : results) {
        v.reserve(n_outputs_);
        for (auto i = 0; i < n_outputs_; ++i) {
            auto& t_shape = snap_->outputs[i].shape;
            std::array<int, 4> shape{ // 确保类型匹配
                1, t_shape[1], t_shape[2], t_shape[3]};
            auto py_arr = build_py_array(shape, current_ptr, capsule);
            v.emplace_back(std::move(py_arr));
            current_ptr += output_sizes[i];
        }
    }

    return results;
}

// 帮助函数，用于在程序结束时清理共享内存
void cleanup_shared_memory() {
    bip::shared_memory_object::remove(SHARED_MEMORY_NAME);
    std::cout << "Cleaned up shared memory segment: " << SHARED_MEMORY_NAME << std::endl;
}




// 在接收进程的 C++ 绑定代码中
py::array_t<float> attach_to_shared_memory(const std::string& name, std::size_t total_len) {
    namespace bip = boost::interprocess;

    // 1. 打开现有的共享内存
    bip::managed_shared_memory segment(bip::open_only, name.c_str());

    // 2. 获取基指针 (这里需要知道数据的名称或偏移量, 使用 find 效率更高)
    // 假设数据从 segment 的开始处分配
    float* p_result_data = reinterpret_cast<float*>(segment.get_address());

    // 3. 创建一个特殊的 capsule，它不删除任何东西，只是表示连接
    auto capsule = py::capsule(p_result_data, [](void* ptr) {
        std::cout << "Receiver capsule deleted. Shared memory remains open." << std::endl;
    });

    // 4. 构建 py::array_t (需要知道原始的 shape 信息来重建 numpy 数组)
    std::vector<std::size_t> shape = { total_len }; // Or the original N x C x H x W shape
    return py::array_t<float>(shape, p_result_data, capsule);
}



// 用construct替代allocate
const char* SHARED_MEMORY_NAME = "MySharedMemorySegment";
const char* DATA_BLOCK_NAME = "ImageDataBlock"; // 给数据块起一个名字

// ... 假设其他变量已定义

ResultType init_results_shared_memory(std::size_t n_imgs, std::size_t n_outputs_, SomeStruct* snap_) {
    
    // ... (计算 total_len 和 total_bytes 的逻辑与之前相同)

    bip::managed_shared_memory segment(bip::create_only, SHARED_MEMORY_NAME, total_bytes + 1024);

    // 关键改变：使用 construct 为数据块命名
    float* p_result_data = segment.construct<float>(DATA_BLOCK_NAME)        // 名称
                                  [total_len]                                // 元素数量
                                  (0.0f);                                    // 初始值 (使用默认构造函数或指定值)

    DLOG(INFO) << fmt::format("Constructed named data block '{}' in shared segment, addr={:x}", 
                              DATA_BLOCK_NAME, (uint64_t)p_result_data);

    // ... (后续的 capsule 创建、build_py_array 和 results 填充逻辑保持不变)

    return results;
}

// 接收进程
namespace py = pybind11;
namespace bip = boost::interprocess;

const char* SHARED_MEMORY_NAME = "MySharedMemorySegment";
const char* DATA_BLOCK_NAME = "ImageDataBlock";

// 接收进程调用的函数，用于连接并返回 py::array_t
py::array_t<float> attach_to_shared_memory() {
    
    try {
        // 1. 打开现有的共享内存段（必须是 open_only）
        bip::managed_shared_memory segment(bip::open_only, SHARED_MEMORY_NAME);

        // 2. 使用 find 方法安全地定位命名的数据块
        std::pair<float*, std::size_t> res = segment.find<float>(DATA_BLOCK_NAME);

        float* p_result_data = res.first;
        std::size_t total_len = res.second;

        if (p_result_data == nullptr) {
            throw std::runtime_error(fmt::format("Could not find data block '{}' in shared memory.", DATA_BLOCK_NAME));
        }

        std::cout << fmt::format("Found data block at {:x} with size {} elements.", (uint64_t)p_result_data, total_len) << std::endl;

        // 3. 创建一个特殊的 capsule，它不负责删除共享内存，只负责记录日志
        auto capsule = py::capsule(p_result_data, [](void* ptr) {
            // 在接收方，我们不执行任何删除操作，因为创建方管理生命周期
            std::cout << "Receiver capsule deleted. Shared memory remains open." << std::endl;
        });

        // 4. 构建 py::array_t
        // 注意：这里我们返回一个扁平的数组。您需要知道原始的 N x C x H x W 形状才能正确重建 numpy 数组的维度。
        std::vector<std::size_t> shape = { total_len }; // 假设你知道原始形状
        
        return py::array_t<float>(shape, p_result_data, capsule);

    } catch (const bip::interprocess_exception& ex) {
        std::cerr << "Error connecting to shared memory: " << ex.what() << std::endl;
        throw;
    }
}


// 销毁对象并释放内存
bool destroyed = segment.destroy<SharedResource>(OBJECT_NAME);
if (destroyed) {
    std::cout << "[Cleanup] Successfully destroyed the C++ object instance." << std::endl;
} else {
    std::cout << "[Cleanup] Could not destroy the object instance." << std::endl;
}
bip::shared_memory_object::remove(SHM_NAME);
