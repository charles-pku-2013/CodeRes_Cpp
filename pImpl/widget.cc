#include "widget.h"

#include <iostream>
#include <type_traits>

struct Widget::Impl {

};

Widget::Widget()
    : pImpl_(std::make_unique<Impl>())
{}

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
