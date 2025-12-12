#include <limits>
#include <iostream>
 
int main() 
{
    std::cout << "type\tlowest\thighest\n";
    std::cout << "int\t"
              << std::numeric_limits<int>::lowest() << '\t'
              << std::numeric_limits<int>::max() << '\n';
    std::cout << "float\t"
              << std::numeric_limits<float>::lowest() << '\t'
              << std::numeric_limits<float>::max() << '\n';
    std::cout << "double\t"
              << std::numeric_limits<double>::lowest() << '\t'
              << std::numeric_limits<double>::max() << '\n';
    std::cout << "size_t\t"
              << std::numeric_limits<std::size_t>::lowest() << '\t'
              << std::numeric_limits<std::size_t>::max() << '\n';

    return 0;
}
