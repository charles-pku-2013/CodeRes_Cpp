#include <iostream>
#include <future>
#include <memory>
#include <string>
#include <vector>

// Sample class with a template member function
class MyClass {
public:
    template <typename T>
    void do_something(T a, const std::string& b) {
        std::cout << "MyClass::do_something<" << typeid(T).name() << "> called with: "
                  << a << " and '" << b << "'\n";
    }
};

// Variadic template function to launch a template member function asynchronously
template <typename T_Class, typename... Args>
auto async_template_member_call(
    std::shared_ptr<T_Class> obj_ptr,
    // Note: Can't pass a pointer to a template member function directly.
    // Use a placeholder or a lambda.
    auto mem_fn,
    Args&&... args)
{
    // The lambda captures the shared_ptr and the member function pointer.
    // This provides a concrete callable object for std::async.
    return std::async(std::launch::async,
        [obj_ptr, mem_fn](Args&&... captured_args) {
            // The template is instantiated here when the lambda is called.
            (obj_ptr.get()->*mem_fn)(std::forward<Args>(captured_args)...);
        },
        std::forward<Args>(args)...
    );
}

int main() {
    auto my_object = std::make_shared<MyClass>();

    // Case 1: Call with an 'int' type for the template
    auto future1 = async_template_member_call(
        my_object,
        &MyClass::do_something<int>, // Explicitly instantiate the template
        42,
        std::string("hello from int")
    );
    future1.get();

    // Case 2: Call with a 'double' type
    auto future2 = async_template_member_call(
        my_object,
        &MyClass::do_something<double>, // Explicitly instantiate for double
        3.14,
        std::string("hello from double")
    );
    future2.get();

    return 0;
}
