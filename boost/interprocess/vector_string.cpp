// combined_example.cpp - 演示写入和读取
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <iostream>

namespace ipc = boost::interprocess;

// 类型定义
using CharAllocator = ipc::allocator<char, ipc::managed_shared_memory::segment_manager>;
using ShmString = ipc::basic_string<char, std::char_traits<char>, CharAllocator>;
using StringAllocator =  ipc::allocator<ShmString, ipc::managed_shared_memory::segment_manager>;
using ShmVector = ipc::vector<ShmString, StringAllocator>;

void write_to_shm() {
    std::cout << "=== Writing to Shared Memory ===" << std::endl;
    
    // 移除之前的内存段
    ipc::shared_memory_object::remove("DemoShm");
    
    // 创建共享内存
    ipc::managed_shared_memory segment(ipc::create_only, "DemoShm", 65536);
    
    // 在共享内存中构造vector
    StringAllocator alloc(segment.get_segment_manager());
    ShmVector* vec = segment.construct<ShmVector>("DataVector")(alloc);
    
    // 添加数据
    CharAllocator char_alloc(segment.get_segment_manager());
    vec->push_back(ShmString("First string", char_alloc));
    vec->push_back(ShmString("Second string", char_alloc));
    vec->push_back(ShmString("Third string", char_alloc));
    
    std::cout << "Written " << vec->size() << " strings to shared memory." << std::endl;
}

void read_from_shm() {
    std::cout << "\n=== Reading from Shared Memory ===" << std::endl;
    
    try {
        // 打开共享内存
        ipc::managed_shared_memory segment(ipc::open_only, "DemoShm");
        
        // 查找vector
        std::pair<ShmVector*, std::size_t> result = segment.find<ShmVector>("DataVector");
        
        if(result.first) {
            ShmVector* vec = result.first;
            std::cout << "Found " << vec->size() << " strings:" << std::endl;
            
            for(size_t i = 0; i < vec->size(); ++i) {
                std::cout << "  " << (*vec)[i] << std::endl;
            }
            
            // 清理
            segment.destroy<ShmVector>("DataVector");
        } else {
            std::cout << "No data found!" << std::endl;
        }
        
        ipc::shared_memory_object::remove("DemoShm");
    }
    catch(const ipc::interprocess_exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    write_to_shm();
    read_from_shm();
    return 0;
}
