#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    std::fstream ofs(argv[1], std::ios::out | std::ios::in | std::ios::binary);

    ofs.seekp(0, std::ios::beg).write("hello", 5);
    return 0;
}
