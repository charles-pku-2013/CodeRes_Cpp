/*
 * c++ -o jsonfmt jsonfmt.cpp -ljsoncpp -std=c++0x -O3
 */
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

int main(int argc, char **argv)
try {
    using namespace std;

    Json::Reader    reader;
    Json::Value     root;

    if (!reader.parse(cin, root)) {
        cerr << "Invalid json format!" << endl;
        exit(-1);
    } // if

    cout << Json::StyledWriter().write(root) << flush;

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << "Unknown exception caught by main!" << std::endl;
    return -1;
}


