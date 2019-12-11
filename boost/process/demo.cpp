/*
c++ -o /tmp/test demo.cpp -lboost_system -lboost_filesystem -std=c++11 -g
 */
#include <iostream>
#include <chrono>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>

namespace bp = boost::process;
namespace fs = boost::filesystem;
using namespace std;

void test_child(const char *cmd) {
    bp::child c("ping www.baidu.com");
    if (c.running())
    { cout << "Child process " << c.id() << " is running" << endl; }

    cout << "Waiting for child " << c.id() << " to finish..." << endl;
    // c.wait();
    // if (c.joinable()) { c.join(); }
    bool status = c.wait_for(std::chrono::seconds(5));
    if (status) {
        cout << "Child " << c.id() << " terminated normally" << endl;
    } else {
        cout << "Child " << c.id() << " is still running, now kill it!" << endl;
        c.terminate();  // SIGKILL
    }
    cout << c.exit_code() << endl;
}

void test_system(const char *cmd) {
    // template<typename ...Args>
    // inline int system(Args && ...args)
    // system("grep -c false /etc/passwd"); //cmd style
    // system("grep", "-c", "false", "/etc/passwd"); //exe-/args-

    // int result = bp::system(cmd);  // will throw exception if cmd not present
    // cout << result << endl;

    // error_code howto
    std::error_code ec;
    int result = bp::system(cmd, ec);
    if (!ec) {
        cout << result << endl;
    } else {
        cout << ec << " " << ec.message() << endl;
    }

    // fs::path p = bp::search_path("file-server");
    // if (!p.empty()) {
        // int result = bp::system(p, "-p", "9999", "-d", ".");
        // int result = bp::system(p, "-p 9999 -d .");  // WRONG
        // cout << result << endl;
    // }

    // async_system 异步版本
}

void test_sync_io(const char *cmd) {
    // bp::system(cmd, bp::std_out > stdout, bp::std_err > stderr, bp::std_in < stdin);
    // bp::system(cmd, bp::std_out > bp::null, bp::std_err > stderr, bp::std_in < stdin);
    // bp::system(cmd, bp::std_out > "/tmp/a.log", bp::std_err > stderr, bp::std_in < stdin);

    bp::ipstream is; //reading pipe-stream
    bp::child c(cmd, bp::std_out > is, bp::std_err > bp::null);

    std::string line;
    while (c.running() && std::getline(is, line))
        cout << line << endl;

    c.wait();
    cout << c.exit_code() << endl;
}

int main(int argc, char **argv) {
    try {
        // test_system(argv[1]);
        // test_child(argv[1]);
        test_sync_io(argv[1]);

    } catch (const std::exception &ex) {
        cerr << "Exception: " << ex.what() << endl;
    } catch (...) {
        cerr << "Unknown error!" << endl;
    }

    return 0;
}


/*
 * The spawn function launches a process and immediately detaches it,
 * so no handle will be returned and the process will be ignored.
 */

