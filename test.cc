
#include <tuple>
#include <utility>
template<typename... T>
void func(T&&... obj) {
    g(std::forward<T>(obj)...);
}

auto func = [](auto&&... arg) {
    g(std::forward<decltype(arg)>(arg)...);
};

vector<int> ret = func();

Foo a; ...
Foo b; ...

b = a;
b = std::move(a);
