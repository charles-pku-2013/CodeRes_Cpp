#include <iostream>
#include <string>
#include <map>
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_format.h"
#include "absl/strings/strip.h"

using namespace std;

int main() {
    std::string s;
    int i = 10;
    double pi = 3.14;
    absl::StrAppend(&s, i, " ", pi); // NOTE!!! No need std::to_string
    cout << s << endl;

    std::map<std::string, double> mm = {{"abc",3.14}, {"def",7.62}, {"canon",5.56}};
    std::string str_mm = absl::StrJoin(mm, " ",
                [](std::string *out, const decltype(mm)::value_type &val){
        absl::StrAppend(out, val.second);  // Use the value directly
    });
    cout << str_mm << endl;

    // NOTE!!! SkipWhitespace 是指整个字符串是空白字符如" ", 不会自动strip
    // std::vector<std::string> v = absl::StrSplit("a , b=c", absl::ByAnyChar(",="), absl::SkipWhitespace());
    // 下面的是预期的效果
    constexpr char SPACES[] = " \t\f\r\v\n";
    std::vector<std::string> v = absl::StrSplit("a , b,,=c",
                absl::ByAnyChar(absl::StrCat(",=", SPACES)), absl::SkipWhitespace());
    cout << absl::StrJoin(v, ",") << endl;

    return 0;
}

