#include <iostream>
#include <vector>
#include <memory_resource>
#include <string>

int main() {
    // 1. Define a large, raw memory buffer
    constexpr size_t POOL_SIZE = 1024 * 1024; // 1 MB
    alignas(std::max_align_t) std::byte pool_buffer[POOL_SIZE];

    // 2. Create a monotonic_buffer_resource using the buffer
    std::pmr::monotonic_buffer_resource buffer_resource(
        pool_buffer,
        POOL_SIZE,
        std::pmr::null_memory_resource() // Fallback if the buffer runs out
    );

    // 3. Use polymorphic allocators tied to this resource
    std::pmr::polymorphic_allocator<int> alloc_int(&buffer_resource);  // 参数用来构造 std::pmr::polymorphic_allocator
    std::pmr::polymorphic_allocator<std::string> alloc_string(&buffer_resource);

    // 4. Use containers with the pmr allocators
    std::vector<int, std::pmr::polymorphic_allocator<int>> vec({1, 2, 3}, &buffer_resource);

    // The vector's elements and its internal management are now using 'buffer_resource'
    vec.push_back(4);

    // You can also allocate other types using their respective allocators, all from the same pool
    auto str_ptr = alloc_string.allocate(1); // Allocates memory for one string object
    alloc_string.deallocate(str_ptr, 1); // Deallocation returns memory to the pool's free list

    // All memory is freed when 'buffer_resource' goes out of scope.
    return 0;
}
