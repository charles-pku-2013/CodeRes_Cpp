#include <iostream>
#include <fstream>
#include <sstream>

bool file2string(const std::string& fname, std::string* content) {
    std::ifstream ifs(fname, std::ios::binary);
    if (!ifs) { return false; }
    std::stringstream strStream;
    strStream << ifs.rdbuf();
    *content = strStream.str();
    return true;
}

int main(int argc, char **argv) {
    std::string s;
    bool ok = file2string(argv[1], &s);
    if (!ok) {
        std::cout << "Failed to read file!" << std::endl;
        return -1;
    }

    std::cout << s << std::flush;

    return 0;
}
