#include <string>
#include <iterator>
#include <vector>
#include <iostream>
#include <algorithm>
#include <array>
 
int main()
{
    // construction uses aggregate initialization
    std::array<int, 3> a1{ {1, 2, 3} }; // double-braces required in C++11 prior to
                                        // the CWG 1270 revision (not needed in C++11
                                        // after the revision and in C++14 and beyond)
    std::vector<std::array<int, 3>> vec_arr;
    vec_arr.emplace_back(a1);

    const std::array<int, 3> *p = nullptr;
    p = &vec_arr[0];
 
    return 0;
}
