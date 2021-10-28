#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
    std::ifstream ifs(argv[1], std::ios::in);
    std::string val;
    ifs >> val;
    if (ifs) {
        cout << val << endl;
        cout << val.length() << endl;
    } else {
        cout << "read val fail!" << endl;
    }
    std::string line;
    if (std::getline(ifs, line)) {
        cout << line << endl;
        cout << line.length() << endl;
    } else {
        cout << "getline fail!" << endl;
    }
    return 0;
}
