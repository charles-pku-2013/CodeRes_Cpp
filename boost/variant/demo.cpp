/*
https://www.boost.org/doc/libs/1_65_0/doc/html/variant.html
 */
#include "boost/variant.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct my_visitor : public boost::static_visitor<int>
{
    int operator()(int i) const
    { return i; }

    int operator()(const std::string & str) const
    { return str.length(); }
};

namespace std {
template<typename T>
std::ostream& operator << (std::ostream &os, const std::vector<T> &data) {
    std::ostringstream oss;
    oss << '[';
    for (auto& d : data) {
        oss << d << ',';
    }
    std::string str = oss.str();
    if (str.back() == ',') { str.pop_back(); }
    str.append(1, ']');
    os << str;
    return os;
}
}  // namespace std

void demo() {
    using DataType = boost::variant<std::string,
                                    double,
                                    int64_t,
                                    bool,
                                    std::vector<float>>;
    std::vector<DataType> data;
    data.emplace_back("hello");
    data.emplace_back("world");
    data.emplace_back<int64_t>(50);
    data.emplace_back<double>(3.1415);
    data.emplace_back(true);
    std::vector<float> arr{1.0, 2.0, 3.1};
    data.emplace_back(std::move(arr));

    for (auto &item : data) {
        cout << item << endl;
    }
}

int main()
{
    demo();
    return 0;

    boost::variant< int, std::string > u("hello world");
    std::cout << u << std::endl; // output: hello world

    int result = boost::apply_visitor( my_visitor(), u );
    std::cout << result << std::endl; // output: 11 (i.e., length of "hello world")

    return 0;
}
