#include "widget.h"

#include <iostream>
#include <type_traits>

struct Widget::Impl {
    void greet(const std::string& name, int n) {
        std::cout << name << "\t" << n << std::endl;
    }
};

Widget::Widget()
    : pImpl_(std::make_unique<Impl>())
{}

// NOTE!!! 构造函数和析构函数必须在源文件里显示定义
// invalid application of ‘sizeof’ to incomplete type ‘Impl’
Widget::~Widget() = default;

Widget::Widget(Widget&& rhs) = default;

Widget& Widget::operator=(Widget&& rhs) = default;

Widget::Widget(const Widget& rhs)
    : pImpl_(std::make_unique<Impl>(*rhs.pImpl_))
{}

Widget& Widget::operator=(const Widget& rhs) {
    *pImpl_ = *rhs.pImpl_;
    return *this;
}

template<typename... Args>
void Widget::_greet(Args&&... args) {
    pImpl_->greet(std::forward<Args>(args)...);
}

void Widget::greet(const std::string& name, int n) {
    this->_greet(name, n);
}
