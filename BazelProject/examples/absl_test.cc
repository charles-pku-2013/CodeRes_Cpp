#include <iostream>
#include <string>
#include <map>
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_format.h"

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

    return 0;
}

