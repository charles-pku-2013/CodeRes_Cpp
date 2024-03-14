#include <iostream>
#include <vector>
#include <map>

template <typename T, typename = void>
struct is_iterable : std::false_type {};

// this gets used only when we can call std::begin() and std::end() on that type
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T&>())),
                                  decltype(std::end(std::declval<T&>()))
                                 >
                  > : std::true_type {};

// Here is a helper:
template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

int main() {
    std::cout << std::boolalpha;
    std::cout << is_iterable_v<std::vector<double>> << '\n';
    std::cout << is_iterable_v<std::map<int, double>> << '\n';
    std::cout << is_iterable_v<double> << '\n';
    struct A { int a; };
    std::cout << is_iterable_v<A> << '\n';

    return 0;
}
