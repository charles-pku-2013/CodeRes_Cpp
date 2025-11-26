#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint> // for uint8_t

// share vector<vector<uint8_t>> between processes

using namespace boost::interprocess;

// Define the allocator for uint8_t in shared memory
typedef allocator<uint8_t, managed_shared_memory::segment_manager> ShmemAllocatorU8;

// Define the inner vector type using the shared memory allocator
typedef vector<uint8_t, ShmemAllocatorU8> ShmemVectorU8;

// Define the allocator for the inner vectors
typedef allocator<ShmemVectorU8, managed_shared_memory::segment_manager> ShmemAllocatorVectorU8;

// Define the outer vector type
typedef vector<ShmemVectorU8, ShmemAllocatorVectorU8> ShmemVectorOfVectors;  // ipc::vector, not std::vector

// Define a name for the shared memory segment and the object within it
const char* SHM_NAME = "MySharedMemorySegment";
const char* OBJECT_NAME = "MyVectorOfVectors";
const size_t SHM_SIZE = 65536; // 64 KB


// Producer
void run_producer() {
    // 1. Remove previous shared memory (if any) and create a new one
    shared_memory_object::remove(SHM_NAME);
    managed_shared_memory segment(create_only, SHM_NAME, SHM_SIZE);

    // 2. Create the shared memory allocators from the segment manager
    ShmemAllocatorU8 alloc_u8_inst(segment.get_segment_manager());
    ShmemAllocatorVectorU8 alloc_vec_u8_inst(segment.get_segment_manager());

    // 3. Construct the vector of vectors in shared memory
    // Use find_or_construct to create and get a pointer to the object
    ShmemVectorOfVectors* shmem_vec_vec = segment.find_or_construct<ShmemVectorOfVectors>(OBJECT_NAME)
                                             (alloc_vec_u8_inst); // Pass the allocator to the constructor

    // 4. Populate the vector of vectors with example data
    for (int i = 0; i < 3; ++i) {
        // Create an inner vector using the correct shared memory allocator
        ShmemVectorU8 inner_vec(alloc_u8_inst);
        for (int j = 0; j < 5; ++j) {
            inner_vec.push_back(i * 10 + j);
        }
        shmem_vec_vec->push_back(inner_vec);
    }

    std::cout << "Producer: Created and populated a vector<vector<uint8_t>> in shared memory." << std::endl;
    std::cout << "Producer: Data size: " << shmem_vec_vec->size() << " outer vectors." << std::endl;

    // Keep the producer running so the consumer can access the memory
    std::cout << "Producer: Press Enter to exit and remove shared memory..." << std::endl;
    std::cin.get();

    // 5. Cleanup (only in the producer/creator process)
    segment.destroy<ShmemVectorOfVectors>(OBJECT_NAME);
    shared_memory_object::remove(SHM_NAME);
    std::cout << "Producer: Cleaned up shared memory." << std::endl;
}


// Consumer
void run_consumer() {
    // 1. Open the existing shared memory segment
    managed_shared_memory segment(open_only, SHM_NAME);

    // 2. Find the already constructed vector of vectors
    ShmemVectorOfVectors* shmem_vec_vec = segment.find<ShmemVectorOfVectors>(OBJECT_NAME).first;

    if (shmem_vec_vec) {
        std::cout << "Consumer: Found vector<vector<uint8_t>> in shared memory." << std::endl;
        std::cout << "Consumer: Data size: " << shmem_vec_vec->size() << " outer vectors." << std::endl;

        // 3. Read and print the data
        for (size_t i = 0; i < shmem_vec_vec->size(); ++i) {
            std::cout << "  Inner vector [" << i << "]: ";
            const auto& inner_vec = (*shmem_vec_vec)[i];
            for (size_t j = 0; j < inner_vec.size(); ++j) {
                std::cout << static_cast<int>(inner_vec[j]) << " ";
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "Consumer: Could not find the object in shared memory." << std::endl;
    }
}
