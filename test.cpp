#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <memory>

namespace std {
std::string to_string(const std::tuple<int, std::string>& tuple);
} // namespace std

namespace std {
std::string to_string(const std::tuple<int, std::string>& tuple) {
    std::stringstream ss;
    ss << "[" << std::get<0>(tuple) << "," << std::get<1>(tuple) << "]";
    return ss.str();
}
} // namespace std


int main(int argc, char **argv) {
    auto tp = std::make_tuple(10, "hello");
    std::cout << std::to_string(tp) << std::endl;

    return 0;
}

