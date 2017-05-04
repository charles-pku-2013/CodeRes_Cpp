/*
 * c++ -o /tmp/test demo.cpp -lglog -lboost_filesystem -lboost_system -std=c++11 -g
 */
#include <iostream>
#include <fstream>
#include <chrono>
#include <boost/filesystem.hpp>
#include <glog/logging.h>

using namespace std;


void test1()
{
    namespace fs = boost::filesystem;
    fs::path p1("../data/conf.json");
    cout << p1 << endl;
    cout << p1.parent_path() << endl;
    cout << p1.filename() << endl;

    fs::path pp(p1.parent_path());
    pp /= p1.filename();
    cout << pp << endl;
}


int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);

    try {
        test1();

    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
    } // try

    return 0;
}
