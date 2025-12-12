#include <boost/scope_exit.hpp>
#include <iostream>

void boostExample() {
    int x = 10;
    std::cout << "进入作用域，x = " << x << std::endl;

    // 使用 BOOST_SCOPE_EXIT 宏来定义清理逻辑
    BOOST_SCOPE_EXIT(&x) { // 捕获局部变量 x
        std::cout << "离开作用域，执行清理，x 变成了 " << x << std::endl;
    } BOOST_SCOPE_EXIT_END

    x = 20;
    std::cout << "执行过程中，x = " << x << std::endl;
} 
// 在这里，宏定义的代码块会自动执行

int main() {
    boostExample();

    return 0;
}
