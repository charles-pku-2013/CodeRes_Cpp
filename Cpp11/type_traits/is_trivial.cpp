#include <iostream>
#include <vector>
#include <map>
#include <type_traits>

int main() {
    using namespace std;

    std::cout << std::boolalpha;
    std::cout << is_trivial_v<std::vector<double>> << '\n';
    std::cout << is_trivial_v<std::map<int, double>> << '\n';
    std::cout << is_trivial_v<std::string> << '\n';
    std::cout << is_trivial_v<double> << '\n';
    struct A { int a; };
    std::cout << is_trivial_v<A> << '\n';

    return 0;
}
