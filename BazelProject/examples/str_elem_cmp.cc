#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <iterator>
#include <algorithm>
#include "absl/strings/str_split.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_format.h"

int main(int argc, char **argv) {
    constexpr char SPACES[] = " \t\f\r\v\n";

    std::string s1, s2;

    std::getline(std::cin, s1);
    std::getline(std::cin, s2);

    std::vector<std::string> v1 = absl::StrSplit(s1, absl::ByAnyChar(SPACES), absl::SkipWhitespace());
    std::vector<std::string> v2 = absl::StrSplit(s2, absl::ByAnyChar(SPACES), absl::SkipWhitespace());

    std::set<std::string> set1(v1.begin(), v1.end());
    std::set<std::string> set2(v2.begin(), v2.end());

    if (set1 != set2) {
        std::cerr << "DIFF!" << std::endl;
        std::vector<std::string> diff1, diff2;
        std::set_difference(set1.begin(), set1.end(),
                            set2.begin(), set2.end(),
                            std::back_inserter(diff1));
        std::set_difference(set2.begin(), set2.end(),
                            set1.begin(), set1.end(),
                            std::back_inserter(diff2));
        std::cerr << absl::StrFormat("1-2: [%s]", absl::StrJoin(diff1, " ")) << std::endl;
        std::cerr << absl::StrFormat("2-1: [%s]", absl::StrJoin(diff2, " ")) << std::endl;
    } else {
        std::cerr << "EQUAL!" << std::endl;
    }

    return 0;
}
