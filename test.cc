#include <iostream>
#include <utility>  // For std::forward

template <typename... Args>
void process_variadic_args(Args&&... args) {
    // Capture the variadic arguments by perfect forwarding
    auto my_lambda = [&args...]() {
        std::cout << "Inside lambda: ";
        // Expand the captured pack when calling another function
        // or performing an operation on each element
        ((std::cout << std::forward<Args>(args) << " "), ...);
        std::cout << std::endl;
    };

    my_lambda();  // Invoke the lambda
}

int main() {
    process_variadic_args(1, 2.5, "hello");
    process_variadic_args('a', true);

    for (int i = 0; i < 10; i++) {
        cout << i << endl;
    }

    return 0;
}
