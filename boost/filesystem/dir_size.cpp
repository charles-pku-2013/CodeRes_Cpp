/*
c++ -o /tmp/test dir_size.cpp -lboost_filesystem -lboost_system -std=c++11 -g
 */
#include <iostream>
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

int main(int argc, char **argv)
{
    size_t size = 0;
    for (fs::recursive_directory_iterator it(argv[1]);
            it != fs::recursive_directory_iterator(); ++it) {
        if(fs::is_regular_file(*it))
            size += fs::file_size(*it);
    }
    cout << size << endl;

    return 0;
}
