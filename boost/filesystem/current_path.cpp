/*
c++ -o /tmp/test current_path.cpp -lboost_filesystem -lboost_system -std=c++11 -g
 */
#include <iostream>
#include <unistd.h>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;
using namespace std;

int main(int argc, char* argv[])
{
    try {
        if (argc > 1) {
            current_path(argv[1]);
        }

        ::sleep(10);
        cout << current_path() << endl;
        for (directory_iterator itr("."); itr != directory_iterator(); ++itr) {
            cout << itr->path().string() << endl;
        }

    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
    } // try

    return 0;
}
