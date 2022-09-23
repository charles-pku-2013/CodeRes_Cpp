#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "tools/run_cmd.h"

int main(int argc, char **argv) {
    namespace fs = boost::filesystem;

    std::cout << "current_path: " << fs::current_path() << std::endl;
    std::string out;
    int retval = tools::run_cmd("cd ~/Documents11 && pwd", &out);
    if (retval != 0) {
        std::cout << "error!" << std::endl;
        return -1;
    }
    boost::trim(out);
    std::cout << out << std::endl;
    std::cout << "current_path: " << fs::current_path() << std::endl;

    return 0;
}
