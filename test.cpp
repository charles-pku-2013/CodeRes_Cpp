#include <iostream>
#include <fstream>
#include <memory>

using namespace std;

int main(int argc, char **argv) {
    std::ifstream in_file(argv[1], std::ios::in);
    if (!in_file) {
        cout << "Cannot open " << argv[1] << endl;
    }

    return 0;
}

