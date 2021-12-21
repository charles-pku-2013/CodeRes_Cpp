/*
c++ -o /tmp/test current_path_work_dir.cpp -lboost_filesystem -lboost_system -std=c++11 -g
 */
#include <iostream>
#include <unistd.h>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
using namespace std;

int main(int argc, char* argv[])
{
    try {
        if (argc > 1) {
            fs::current_path(argv[1]);
        }
        cout << "CWD: " << fs::current_path() << endl;
        for (fs::directory_iterator itr("."); itr != fs::directory_iterator(); ++itr) {
            cout << itr->path().string() << endl;
        }

    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
        return -1;
    }

    return 0;
}
