#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <cstdint>
#include <iostream>
#include <string>

namespace bip = boost::interprocess;

// 定义共享内存分配器类型
typedef bip::allocator<uint8_t, bip::managed_shared_memory::segment_manager> Uint8Allocator;
typedef bip::allocator<uint32_t, bip::managed_shared_memory::segment_manager> Uint32Allocator;

// 定义共享内存中的向量类型
typedef bip::vector<uint8_t, Uint8Allocator> ShmVectorUint8;
typedef bip::vector<uint32_t, Uint32Allocator> ShmVectorUint32;

// 共享内存中的Matrix结构体
struct ShmMatrix {
    uint32_t n_dim;
    ShmVectorUint32 shape;
    ShmVectorUint8 data;
    
    // 构造函数，接受分配器
    ShmMatrix(Uint32Allocator alloc)
        : shape(alloc), data(alloc.get_segment_manager()) {
        n_dim = 0;
    }
    
    // 从普通Matrix复制数据（可选）
    void copy_from(const Matrix& mat, Uint32Allocator alloc) {
        n_dim = mat.n_dim;
        shape.assign(mat.shape.begin(), mat.shape.end());
        data.assign(mat.data.begin(), mat.data.end());
    }
    
    // 计算数据大小
    size_t total_elements() const {
        size_t count = 1;
        for(size_t i = 0; i < shape.size(); ++i) {
            count *= shape[i];
        }
        return count;
    }
};

// 定义Matrix的分配器类型
typedef bip::allocator<ShmMatrix, bip::managed_shared_memory::segment_manager> MatrixAllocator;

// 定义共享内存中的MatrixSet类型
typedef bip::vector<ShmMatrix, MatrixAllocator> ShmMatrixSet;


// writer
#include "shm_types.hpp"  // 包含上面的类型定义
#include <vector>
#include <string>

// 普通Matrix定义（用于源数据）
struct Matrix {
    uint32_t n_dim;
    std::vector<uint32_t> shape;
    std::vector<uint8_t> data;
};

using MatrixSet = std::vector<Matrix>;

void write_matrix_set() {
    const char* shm_name = "MatrixSharedMemory";
    const size_t shm_size = 1024 * 1024;  // 1MB
    
    try {
        // 1. 移除之前可能存在的共享内存
        bip::shared_memory_object::remove(shm_name);
        
        // 2. 创建共享内存段
        bip::managed_shared_memory segment(bip::create_only, shm_name, shm_size);
        
        // 3. 获取分配器
        Uint32Allocator uint32_alloc(segment.get_segment_manager());
        MatrixAllocator matrix_alloc(segment.get_segment_manager());
        
        // 4. 在共享内存中构造MatrixSet
        ShmMatrixSet* matrix_set = segment.construct<ShmMatrixSet>("MatrixSet")(matrix_alloc);
        
        // 5. 创建一些示例数据
        // Matrix 1: 2x3 矩阵
        {
            Matrix mat1;
            mat1.n_dim = 2;
            mat1.shape = {2, 3};
            mat1.data = {1, 2, 3, 4, 5, 6};
            
            // 在共享内存中创建Matrix
            matrix_set->push_back(ShmMatrix(uint32_alloc));
            ShmMatrix& shm_mat = matrix_set->back();
            shm_mat.copy_from(mat1, uint32_alloc);
        }
        
        // Matrix 2: 1x4 向量
        {
            Matrix mat2;
            mat2.n_dim = 1;
            mat2.shape = {4};
            mat2.data = {10, 20, 30, 40};
            
            matrix_set->push_back(ShmMatrix(uint32_alloc));
            ShmMatrix& shm_mat = matrix_set->back();
            shm_mat.copy_from(mat2, uint32_alloc);
        }
        
        // Matrix 3: 2x2x2 张量
        {
            Matrix mat3;
            mat3.n_dim = 3;
            mat3.shape = {2, 2, 2};
            mat3.data = {1, 2, 3, 4, 5, 6, 7, 8};
            
            matrix_set->push_back(ShmMatrix(uint32_alloc));
            ShmMatrix& shm_mat = matrix_set->back();
            shm_mat.copy_from(mat3, uint32_alloc);
        }
        
        std::cout << "Writer: Created " << matrix_set->size() << " matrices in shared memory" << std::endl;
        std::cout << "Writer: Press Enter to continue..." << std::endl;
        std::cin.get();
        
        // 注意：不要在这里销毁segment，reader还需要访问
        
    } catch(const std::exception& e) {
        std::cerr << "Writer error: " << e.what() << std::endl;
        bip::shared_memory_object::remove(shm_name);
    }
}

int main() {
    write_matrix_set();
    return 0;
}

// reader
#include "shm_types.hpp"
#include <iostream>
#include <iomanip>

void read_matrix_set() {
    const char* shm_name = "MatrixSharedMemory";
    
    try {
        // 1. 打开共享内存
        bip::managed_shared_memory segment(bip::open_only, shm_name);
        
        // 2. 查找MatrixSet
        std::pair<ShmMatrixSet*, bip::managed_shared_memory::size_type> result;
        result = segment.find<ShmMatrixSet>("MatrixSet");
        
        if(!result.first) {
            std::cerr << "Reader: MatrixSet not found!" << std::endl;
            return;
        }
        
        ShmMatrixSet* matrix_set = result.first;
        
        // 3. 读取并显示所有矩阵
        std::cout << "Reader: Found " << matrix_set->size() << " matrices:" << std::endl;
        std::cout << "========================================" << std::endl;
        
        for(size_t i = 0; i < matrix_set->size(); ++i) {
            const ShmMatrix& matrix = (*matrix_set)[i];
            
            std::cout << "Matrix " << i << ":" << std::endl;
            std::cout << "  Dimensions: " << matrix.n_dim << std::endl;
            std::cout << "  Shape: [";
            for(size_t j = 0; j < matrix.shape.size(); ++j) {
                std::cout << matrix.shape[j];
                if(j < matrix.shape.size() - 1) std::cout << ", ";
            }
            std::cout << "]" << std::endl;
            
            std::cout << "  Data (" << matrix.data.size() << " elements): ";
            for(size_t j = 0; j < matrix.data.size(); ++j) {
                std::cout << static_cast<int>(matrix.data[j]);
                if(j < matrix.data.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
            
            // 验证数据大小是否匹配形状
            size_t expected_size = matrix.total_elements();
            if(matrix.data.size() == expected_size) {
                std::cout << "  ✓ Data size matches shape" << std::endl;
            } else {
                std::cout << "  ✗ Data size mismatch! Expected " << expected_size 
                          << " elements, got " << matrix.data.size() << std::endl;
            }
            
            std::cout << std::endl;
        }
        
        // 4. 清理（可选）
        segment.destroy<ShmMatrixSet>("MatrixSet");
        bip::shared_memory_object::remove(shm_name);
        
        std::cout << "Reader: Cleaned up shared memory" << std::endl;
        
    } catch(const std::exception& e) {
        std::cerr << "Reader error: " << e.what() << std::endl;
    }
}

int main() {
    read_matrix_set();
    return 0;
}
