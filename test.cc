#include <iostream>
#include <vector>
#include <numeric>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/process/child.hpp>
#include <boost/process/exe.hpp>
#include <boost/process/args.hpp>

using namespace boost::interprocess;

// --- 类型定义 (与之前相同) ---

typedef managed_shared_memory::segment_manager SegmentManager;

typedef allocator<uint8_t, SegmentManager> ShmUint8Allocator;
typedef boost::interprocess::vector<uint8_t, ShmUint8Allocator> ShmUint8Vector;

typedef allocator<ssize_t, SegmentManager> ShmSsizeTAllocator;
typedef boost::interprocess::vector<ssize_t, ShmSsizeTAllocator> ShmSsizeTVector;

struct ShmImageData {
    ShmSsizeTVector shape_;
    ShmUint8Vector data_;
    ShmImageData(const ShmSsizeTAllocator& s_alloc, const ShmUint8Allocator& d_alloc)
        : shape_(s_alloc), data_(d_alloc) {}
};

typedef allocator<ShmImageData, SegmentManager> ShmImageDataAllocator;
typedef boost::interprocess::vector<ShmImageData, ShmImageDataAllocator> ShmImageDataVector;

// --- 共享内存资源的名称 ---
const char* SHM_NAME = "MyImageDataShm_NoControlBlock";
const char* IMAGE_VECTOR_NAME = "ShmImageDataVectorInstance";
// 新增：互斥锁的名称
const char* MUTEX_NAME = "ShmMutex";


// --- 父进程逻辑 (写入数据) ---
void parent_process() {
    std::cout << "Parent: Starting up. Creating shared memory segment..." << std::endl;

    shared_memory_object::remove(SHM_NAME);
    managed_shared_memory segment(create_only, SHM_NAME, 65536);

    const ShmImageDataAllocator alloc_inst(segment.get_segment_manager());
    const ShmSsizeTAllocator s_alloc_inst(segment.get_segment_manager());
    const ShmUint8Allocator d_alloc_inst(segment.get_segment_manager());

    // 1. 在共享内存中构造互斥锁
    interprocess_mutex* mutex_ptr = segment.construct<interprocess_mutex>(MUTEX_NAME)();

    // 2. 在共享内存中构造 ShmImageDataVector
    ShmImageDataVector* images_vec = segment.construct<ShmImageDataVector>(IMAGE_VECTOR_NAME)(alloc_inst);

    {
        // 加锁，使用我们单独创建的互斥锁
        scoped_lock<interprocess_mutex> lock(*mutex_ptr);
        std::cout << "Parent: Lock acquired. Populating data..." << std::endl;

        for (int i = 0; i < 3; ++i) {
            ShmImageData new_img(s_alloc_inst, d_alloc_inst);
            new_img.shape_.push_back(10 + i);
            new_img.shape_.push_back(20 + i);
            
            size_t data_size = (10 + i) * (20 + i); // Calculate size correctly
            new_img.data_.resize(data_size);
            std::iota(new_img.data_.begin(), new_img.data_.end(), static_cast<uint8_t>(i * 10));

            // 添加到共享内存的 vector 中
            images_vec->push_back(std::move(new_img));
        }

        std::cout << "Parent: Data populated. Releasing lock." << std::endl;
    } // 锁自动释放

    // 启动子进程
    namespace bp = boost::process;
    std::cout << "Parent: Launching child process..." << std::endl;
    bp::child c(bp::exe(bp::self), "--child");
    
    c.wait(); 
    std::cout << "Parent: Child process finished." << std::endl;

    // 清理共享内存
    shared_memory_object::remove(SHM_NAME);
    std::cout << "Parent: Shared memory cleaned up. Exiting." << std::endl;
}


// --- 子进程逻辑 (读取数据) ---
void child_process() {
    std::cout << "Child: Starting up. Opening shared memory segment..." << std::endl;

    managed_shared_memory segment(open_only, SHM_NAME);

    // 1. 查找互斥锁实例
    interprocess_mutex* mutex_ptr = segment.find<interprocess_mutex>(MUTEX_NAME).first;
    if (!mutex_ptr) {
        std::cerr << "Child: Error: Could not find mutex in shared memory." << std::endl;
        return;
    }

    // 2. 查找 ShmImageDataVector 实例
    ShmImageDataVector* images_vec = segment.find<ShmImageDataVector>(IMAGE_VECTOR_NAME).first;
     if (!images_vec) {
        std::cerr << "Child: Error: Could not find image vector in shared memory." << std::endl;
        return;
    }

    {
        // 加锁，使用找到的互斥锁
        scoped_lock<interprocess_mutex> lock(*mutex_ptr);
        std::cout << "Child: Lock acquired. Reading data..." << std::endl;

        std::cout << "Child: Found " << images_vec->size() << " images." << std::endl;

        for (size_t i = 0; i < images_vec->size(); ++i) {
            const auto& img = (*images_vec)[i];
            std::cout << "  Image [" << i << "]:" << std::endl;
            std::cout << "    Shape: (";
            for (size_t j = 0; j < img.shape_.size(); ++j) {
                std::cout << img.shape_[j] << (j == img.shape_.size() - 1 ? "" : ", ");
            }
            std::cout << ")" << std::endl;
            std::cout << "    Data Size: " << img.data_.size() << " bytes" << std::endl;
            // 打印前几个字节以验证数据
            std::cout << "    Data Start (first 5 bytes): ";
            for (int k = 0; k < std::min((int)img.data_.size(), 5); ++k) {
                 std::cout << (int)img.data_[k] << " ";
            }
            std::cout << "..." << std::endl;
        }

        std::cout << "Child: Data read complete. Releasing lock. Exiting." << std::endl;
    } // 锁自动释放
}


// --- Main 函数 ---
int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv) == "--child") {
        child_process();
    } else {
        parent_process();
    }
    return 0;
}
