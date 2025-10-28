
#include <iostream>
#include <vector>
#include <future>
#include <thread>

std::vector<int> expensive_computation() {
    std::cout << "Starting heavy computation..." << std::endl;
    std::vector<int> large_vector;
    large_vector.reserve(100000000);
    for (int i = 0; i < 100000000; ++i) {
        large_vector.push_back(i);
    }
    std::cout << "Computation complete " << (uint64_t)(large_vector.data()) << std::endl;
    return large_vector; // Move semantics applied here
}

int main() {
    std::cout << "Main thread launching async task." << std::endl;
    std::future<std::vector<int>> future_result = std::async(std::launch::async, expensive_computation);

    std::cout << "Main thread doing other work..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Main thread waiting for result." << std::endl;
    auto result_vector = future_result.get(); // Move construction from the future
    std::cout << "Received vector of size: " << result_vector.size() << " " << (uint64_t)(result_vector.data()) << std::endl;

    // WRONG you can only get once
    // std::vector<int> result_vector1 = future_result.get(); // Move construction from the future
    // std::cout << "Received vector of size: " << result_vector1.size() << " " << (uint64_t)(result_vector1.data()) << std::endl;

    return 0;
}
