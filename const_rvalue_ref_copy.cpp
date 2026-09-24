#include <iostream>
#include <utility>

class Widget {
public:
    Widget() {
        std::cout << "Default Constructor\n";
    }

    // 1. 拷贝构造函数：接收 const Widget&
    Widget(const Widget& other) {
        std::cout << "Copy Constructor (Deep Copy!)\n";
    }

    // 2. 移动构造函数：接收 Widget&& (注意：是非 const 的右值引用)
    Widget(Widget&& other) noexcept {
        std::cout << "Move Constructor (Resource Stolen!)\n";
    }
};

int main() {
    Widget a;
    
    std::cout << "--- 1. 非 const 对象 move ---\n";
    Widget b = std::move(a); // 触发 Move Constructor

    const Widget c;
    std::cout << "--- 2. const 对象 move ---\n";
    Widget d = std::move(c); // 触发 Copy Constructor！

    return 0;
}
