#include <iostream>
#include <initializer_list>

// Function that takes an std::initializer_list
void print_list(std::initializer_list<int> list) {
    std::cout << "Received " << list.size() << " elements:" << std::endl;
    for (auto& value : list) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

template<typename List>
void print_list_template(List&& list) {
    std::cout << "Received " << list.size() << " elements:" << std::endl;
    for (auto& value : list) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Call the function with a brace-enclosed list of values
    print_list({10, 20, 30, 40, 50});
    print_list_template<std::initializer_list<int>>({10, 20, 30, 40, 50});  // 必须显示指定模板参数
    return 0;
}
