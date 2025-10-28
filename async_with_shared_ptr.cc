#include <iostream>
#include <future>
#include <memory> // For std::shared_ptr

class MyClass {
public:
    void doSomething(int value) {
        std::cout << "MyClass::doSomething called with value: " << value << std::endl;
    }

    int calculateSum(int a, int b) {
        return a + b;
    }
};

int main() {
    // Create a shared_ptr to an instance of MyClass
    std::shared_ptr<MyClass> myObject = std::make_shared<MyClass>();

    // Call a void member function asynchronously
    std::future<void> future1 = std::async(std::launch::async, &MyClass::doSomething, myObject, 10);
    future1.get(); // Wait for the async operation to complete

    // Call a member function that returns a value asynchronously
    std::future<int> future2 = std::async(std::launch::async, &MyClass::calculateSum, myObject, 5, 7);
    int result = future2.get(); // Get the result from the async operation
    std::cout << "Result of calculateSum: " << result << std::endl;

    return 0;
}
