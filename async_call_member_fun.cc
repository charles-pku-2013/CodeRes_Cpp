#include <future>
#include <memory>
#include <iostream>
#include <utility> // For std::forward

class MyClass {
public:
    void printMessage(const std::string& msg) {
        std::cout << "Message from MyClass: " << msg << std::endl;
    }

    void addNumbers(int a, int b) {
        std::cout << "Sum from MyClass: " << (a + b) << std::endl;
    }
};

template<typename T, typename Func, typename... Args>
auto asyncCallMemberFunction(std::shared_ptr<T> obj_ptr, Func func, Args&&... args) {
    // Ensure the shared_ptr is valid before attempting to dereference
    if (!obj_ptr) {
        throw std::runtime_error("Attempted to call member function on a null shared_ptr.");
    }
    
    // std::async with a member function requires the object instance as the second argument
    // followed by the member function arguments.
    return std::async(std::launch::async, func, obj_ptr.get(), std::forward<Args>(args)...);
}

template<typename... Args>
void test(int a, Args&&... args) {
    std::shared_ptr<MyClass> myObject = std::make_shared<MyClass>();

    // Call printMessage asynchronously
    auto future1 = asyncCallMemberFunction(myObject, &MyClass::printMessage, "Hello from async!");
    future1.get(); // Wait for the async operation to complete

    // Call addNumbers asynchronously
    auto future2 = asyncCallMemberFunction(myObject, &MyClass::addNumbers, a, std::forward<Args>(args)...);
    future2.get(); // Wait for the async operation to complete
}

int main() {
    test(10, 29);

    return 0;
}


