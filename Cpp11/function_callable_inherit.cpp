#include <iostream>
#include <string>
#include <functional>

struct Base : public std::function<int(const std::string&, void*)> {
    virtual int operator()(const std::string &conf, void *args) const {
        std::cerr << "Base call" << std::endl;
        return 0;
    }
};

struct Derived : public Base {
    int operator()(const std::string &conf, void *args) const override {
        std::cerr << "Derived call" << std::endl;
        return 0;
    }
};


int main() {
    Base *p = new Derived;
    if (p && *p) {
        (*p)("123", nullptr);
    }

    // Base op = nullptr;
    std::function<int(const std::string&, void*)> op = nullptr;

    return 0;
}
