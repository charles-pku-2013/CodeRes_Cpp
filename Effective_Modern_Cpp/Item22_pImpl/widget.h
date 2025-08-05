#pragma once
#include <memory>

class Widget {
 public:
    Widget();
    ~Widget();

    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs);

    Widget(Widget&& rhs);
    Widget& operator=(Widget&& rhs);

    void greet(const std::string& name, int n);

 protected:
    struct Impl;
    std::unique_ptr<Impl> pImpl_;

    template<typename... Args>
    void _greet(Args&&... args);
};
