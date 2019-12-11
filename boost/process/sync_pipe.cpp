/*
c++ -o /tmp/test sync_pipe.cpp -lboost_system -lboost_filesystem -std=c++11 -g
 */
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>

namespace bp = boost::process;
namespace fs = boost::filesystem;
using namespace std;

std::vector<std::string> read_demangled_outline(const std::string & file)
{
    bp::pipe p;
    bp::ipstream is;

    std::vector<std::string> outline;

    //we just use the same pipe, so the
    bp::child nm(bp::search_path("nm"), file,  bp::std_out > p);
    bp::child filt(bp::search_path("c++filt"), bp::std_in < p, bp::std_out > is);

    std::string line;
    while (filt.running() && std::getline(is, line)) //when nm finished the pipe closes and c++filt exits
        outline.push_back(line);

    nm.wait();
    filt.wait();
}

// 一个进程输出到另一个进程输入
std::vector<std::string> read_outline(const char *cmd)
{
    bp::ipstream is; //reading pipe-stream
    bp::child c(cmd, bp::std_out > is);

    std::vector<std::string> data;
    std::string line;

    while (c.running() && std::getline(is, line) && !line.empty())
        data.push_back(line);

    c.wait();

    return std::move(data);
}

int main(int argc, char **argv) {
    try {
        auto data = read_outline(argv[1]);
        for (const std::string &s : data) {
            cout << s << endl;
        }

    } catch (const std::exception &ex) {
        cerr << "Exception: " << ex.what() << endl;
    } catch (...) {
        cerr << "Unknown error!" << endl;
    }

    return 0;
}
