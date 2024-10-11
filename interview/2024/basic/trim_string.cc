#include <iostream>
#include <string>

int main() {
    using namespace std;

    const char SPACES[] = " \t\f\r\v\n";

    // std::string s(" hello world ");
    std::string s("hello world   ");
    // std::string s("      ");
    // std::string s;
    s.erase(0, s.find_first_not_of(SPACES)).erase(s.find_last_not_of(SPACES) + 1);  // NOTE string erase(pos, count) 可以连续调用
    cout << s << endl;
    cout << s.length() << endl;

    return 0;
}


