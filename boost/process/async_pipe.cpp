/*
c++ -o /tmp/test async_pipe.cpp -lboost_system -lboost_filesystem -std=c++11 -g
 */
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>

namespace bp = boost::process;
namespace fs = boost::filesystem;
using namespace std;

// 一个进程输出到另一个进程输入
int test(const char *cmd, std::vector<char> *buf)
{
    boost::asio::io_service ios;
    bp::async_pipe ap(std::ref(ios));
    bp::child c(cmd, bp::std_out > ap);
    boost::asio::async_read(ap, boost::asio::buffer(*buf),
                    [](const boost::system::error_code &ec, std::size_t size){});
    ios.run();
    c.wait();
    return c.exit_code();
}

int test_1(const char *cmd)
{
    boost::asio::io_service ios;
    std::vector<char> buf;
    bp::child c(bp::search_path("g++"), "main.cpp", bp::std_out > boost::asio::buffer(buf), ios);
    ios.run();
    c.wait();
    return c.exit_code();
}

int main(int argc, char **argv) {
    try {
        std::vector<char> buf;
        int result = test(argv[1], &buf);
        cout << result << endl;
        cout << std::string(&buf[0], buf.size()) << endl;

    } catch (const std::exception &ex) {
        cerr << "Exception: " << ex.what() << endl;
    } catch (...) {
        cerr << "Unknown error!" << endl;
    }

    return 0;
}
