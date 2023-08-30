#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>
#include "absl/strings/str_join.h"

template<typename MapType>
std::vector<typename MapType::key_type> get_keys(const MapType &m) {
    std::vector<typename MapType::key_type> keys;
    std::transform(m.begin(), m.end(), std::back_inserter(keys),
            [](const typename MapType::value_type& item){ return item.first; });
    return keys;
}

int main() {
    std::map<int, std::string> m;
    m[1] = "a";
    m[2] = "a";
    m[3] = "a";
    std::vector<int> arr = get_keys(m);
    std::cout << absl::StrJoin(arr, ",") << std::endl;

    return 0;
}

