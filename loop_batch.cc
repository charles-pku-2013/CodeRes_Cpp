#include <iostream>
#include <vector>
#include <algorithm> // For std::min

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    int batch_size = 4;

    for (int i = 0; i < data.size(); i += batch_size) {
        // Calculate the end index for the current batch
        int end_index = std::min(i + batch_size, static_cast<int>(data.size()));

        // Process the current batch of elements
        std::cout << "Processing batch from index " << i << " to " << end_index << ": ";
        for (int j = i; j < end_index; ++j) {
            std::cout << data[j] << " ";
            // Perform your batch operation here, e.g., send to a function,
            // perform calculations, etc.
        }
        std::cout << std::endl;
    }

    return 0;
}
