/*
c++ -o /tmp/test recursive_iter.cpp -lboost_filesystem -lboost_system -std=c++11 -g
 */
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
namespace fs = boost::filesystem;

int main(int argc, char **argv)
{
    namespace fs = boost::filesystem;
    fs::recursive_directory_iterator dir(argv[1], fs::symlink_option::recurse), end;
    for (; dir != end; ++dir) {
        std::string path_name = dir->path().string();
        if (boost::ends_with(path_name, ".so")) {
            cout << path_name << endl;
        }
    }

    return 0;
}

// list files
for (directory_iterator itr(path_ss); itr!=directory_iterator(); ++itr)
{
    cout << itr->path().filename() << ' '; // display filename only
    if (is_regular_file(itr->status())) cout << " [" << file_size(itr->path()) << ']';  // 显示文件大小
    cout << '\n';
    // NOTE *itr type is fs::directory_entry, to get its name: itr->path().string()
}
