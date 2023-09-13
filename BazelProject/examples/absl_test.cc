#include <iostream>
#include <string>
#include <map>
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_format.h"
#include "absl/strings/strip.h"

using namespace std;

void test_StrAppend() {
    std::string s;
    int i = 10;
    double pi = 3.14;
    absl::StrAppend(&s, i, " ", pi); // NOTE!!! No need std::to_string
    cout << s << endl;
}

void test_StrJoin_map() {
    std::map<std::string, double> mm = {{"abc",3.14}, {"def",7.62}, {"canon",5.56}};

    // fully customize how to print value
    std::string str_map_value = absl::StrJoin(mm, " ",
                [](std::string *out, const decltype(mm)::value_type &val){
        absl::StrAppend(out, val.second);  // Use the value directly
    });
    cout << str_map_value << endl;

    std::string str_map_1 = absl::StrJoin(mm, ":",
                [](std::string *out, const decltype(mm)::value_type &val){
        absl::StrAppend(out, absl::StrFormat("%s{%lf}", val.first, val.second));
    });
    cout << str_map_1 << endl;

    // Standard
    std::string str_map = absl::StrJoin(mm, ",", absl::PairFormatter("="));
    cout << str_map << endl;
}

void test_StrSplit_1() {
    // NOTE!!! SkipWhitespace 是指整个字符串是空白字符如" ", 不会自动strip
    // std::vector<std::string> v = absl::StrSplit("a , b=c", absl::ByAnyChar(",="), absl::SkipWhitespace());
    // 下面的是预期的效果
    constexpr char SPACES[] = " \t\f\r\v\n";
    std::vector<std::string> v = absl::StrSplit("a , b,,=c",
                absl::ByAnyChar(absl::StrCat(",=", SPACES)), absl::SkipWhitespace());
    cout << absl::StrJoin(v, ",") << endl;  // a,b,c
}

void test_StrSplit_2() {
    // Demonstrates a "smart" split to std::map using two separate calls to
    // absl::StrSplit. One call to split the records, and another call to split
    // the keys and values. This also uses the Limit delimiter so that the
    // std::string "a=b=c" will split to "a" -> "b=c".
    std::map<std::string, std::string> m;
    for (absl::string_view sp : absl::StrSplit("a=b=c,d=e,f=,g", ',')) {
        m.insert(absl::StrSplit(sp, absl::MaxSplits('=', 1)));
    }
    cout << absl::StrJoin(m, ",", absl::PairFormatter(":")) << endl;
}

void test_Strip_Trim() {
    // like boost::trim()
    std::string test2 = "\t  \f\r\n\vfoo \t\f\r\v\n";
    absl::StripAsciiWhitespace(&test2);
    cout << test2 << endl;
}

int main() {
    // test_StrAppend();
    test_StrJoin_map();
    // test_StrSplit_1();
    // test_StrSplit_2();
    // test_Strip_Trim();

    return 0;
}

