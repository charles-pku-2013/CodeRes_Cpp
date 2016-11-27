/*
 * c++ -o /tmp/test test.cpp -lboost_filesystem -lboost_system -std=c++11 -g
 */
#include <iostream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
using namespace std;

void check_exist(const std::string &fname)
{
    if (fs::exists(fname))
        cout << "Exists" << endl;
    else
        cout << "Not exists" << endl;
}

int main(int argc, char* argv[])
{
    try {
        if (argc > 1)
            check_exist( argv[1] );

    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
    } // try

    return 0;
}
