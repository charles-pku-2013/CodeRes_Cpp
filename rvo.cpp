#include <iostream>
#include <utility>

class Widget {
public:
    Widget() { std::cout << "1. Default Constructor\n"; }
    Widget(const Widget&) { std::cout << "2. Copy Constructor\n"; }
    Widget(Widget&&) noexcept { std::cout << "3. Move Constructor\n"; }
    ~Widget() { std::cout << "4. Destructor\n"; }
};

Widget testNRVO() {
    Widget w;
    return w; // 正确：优先触发 NRVO
}

Widget testBadMove() {
    Widget w;
    return std::move(w); // 错误：阻断 NRVO
}

int main() {
    std::cout << "=== 测试正确写法 (return w) ===\n";
    Widget a = testNRVO();

    std::cout << "\n=== 测试误用 move (return std::move(w)) ===\n";
    Widget b = testBadMove();
}

/*
 * === 测试正确写法 (return w) ===
 * 1. Default Constructor    <-- 直接在外部 a 构造，无任何拷贝/移动！
 * 
 * === 测试误用 move (return std::move(w)) ===
 * 1. Default Constructor    <-- 内部构造局部变量
 * 3. Move Constructor       <-- 强行触发移动给外部 b
 * 4. Destructor             <-- 析构局部变量
 */
