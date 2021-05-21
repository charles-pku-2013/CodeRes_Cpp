#include <iostream>
#include <string>
#include <unordered_map>

enum OSType : int32_t {
    Linux,
    Apple,
    Windows,
};

namespace {
std::unordered_map<int32_t, std::string> os_type_str = {
    {Linux, "Linux"},
    {Apple, "Apple"},
    {Windows, "Windows"},
};

std::string os_type_2_str(int32_t type) {
    switch (type) {
        case Linux: return "Linux";
        case Apple: return "Apple";
        case Windows: return "Windows";
    }
    return "Unknown";
}
}  // namespace

int main() {
    int32_t os_type = Apple;
    std::cout << os_type_str[os_type] << std::endl;
    std::cout << os_type_2_str(os_type) << std::endl;

    return 0;
}

