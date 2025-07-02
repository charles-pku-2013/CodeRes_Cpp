#include <iostream>
#include <fstream>
#include <string>
#include <boost/format.hpp>

int main(int argc, char **argv) {
    using namespace std;

    std::ifstream ifs("/dev/random", std::ios::in);

    std::string s(256, 0);
    ifs.read(s.data(), 256);
    // cout << s << endl;

    for (uint8_t ch : s) {
        printf("%02x ", ch);
    }
    cout << endl;

    return 0;
}
