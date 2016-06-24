/*
 * c++ -o /tmp/test test.cpp -lboost_filesystem -lboost_system -lglog -std=c++11 -g
 */
#include <iostream>
#include <fstream>
#include <chrono>
#include <boost/filesystem.hpp>
#include <glog/logging.h>


using namespace boost::filesystem;
using namespace std;

/*
 * 更改当前工作路径只会影响之后打开文件，不影响已经打开的文件
 */
void test1()
{
    cout << "current path: " << current_path() << endl;

    LOG(INFO) << "log record 1";
    LOG(INFO) << "log record 2";
    LOG(INFO) << "log record 3";

    current_path("..");
    cout << "current path: " << current_path() << endl;

    LOG(INFO) << "log record 4";
    LOG(INFO) << "log record 5";
    LOG(INFO) << "log record 6";
}


void test2()
{
    ofstream ofs("test.log", ios::out);
    ofs << "current path: " << current_path() << endl;
    current_path("..");
    ofs << "current path: " << current_path() << endl;
}


int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);

    try {
        // test1();
        test2();

    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
    } // try

    return 0;
}
