#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

void test_next_permunation() {
    std::string s = "abc";
    do {
        std::cout << s << '\n';
    } while (std::next_permutation(s.begin(), s.end()));
    std::cout << s << '\n';
}

void test_is_permunation() {
    std::string s("abc");
    std::string t("bca");
    cout << std::is_permutation(t.begin(), t.end(), s.begin()) << endl;
}
 
int main() {
    // test_next_permunation();
    test_is_permunation();

    return 0;
}
