#include <iostream>
#include <vector>
#include <algorithm> // For std::min

using namespace std;

template<typename C>
void split_into_batches(const C& arr, std::size_t batch_size) {
    std::size_t this_batch_size = batch_size;

    for (std::size_t i = 0; i < arr.size(); i += this_batch_size) {
        this_batch_size = std::min(arr.size() - i, batch_size);
        for (std::size_t j = 0; j < this_batch_size; ++j) {
            cout << arr[i + j] << " ";
        }
        cout << endl;
    }
}

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    split_into_batches(data, 0);
    return 0;

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

    std::cout << std::endl;

    // method 2
    std::size_t n_batches = (data.size() / batch_size) + (data.size() % batch_size ? 1 : 0);
    for (std::size_t b = 0; b < n_batches; ++b) {
        std::size_t start = b * batch_size;
        std::size_t end = std::min(data.size(), start + batch_size);
        std::cout << "Processing batch from index " << start << " to " << end << ": ";
        for (std::size_t j = start; j < end; ++j) {
            std::cout << data[j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
