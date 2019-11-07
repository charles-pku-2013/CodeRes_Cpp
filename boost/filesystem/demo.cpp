/*
c++ -o /tmp/test demo.cpp -lboost_filesystem -lboost_system -std=c++11 -g
 */
#include <iostream>
#include <fstream>
#include <chrono>
#include <boost/filesystem.hpp>
// #include <glog/logging.h>

using namespace std;

void test1()
{
    namespace fs = boost::filesystem;
    // fs::path p1("../data/conf.json");
    fs::path p1("../../Cpp11/type_traits/is_destructible.cpp");
    cout << p1 << endl;
    cout << p1.parent_path() << endl;
    cout << p1.filename() << endl;
    cout << p1.stem() << endl; // 主文件名
    cout << p1.extension() << endl;
    cout << p1.relative_path() << endl;
    cout << fs::is_directory(p1) << endl;  // TODO symlink
    cout << fs::is_regular_file(p1) << endl;
    cout << fs::exists(p1) << endl;
    // cout << p1.root_path() << endl;
    // cout << p1.root_name() << endl;

    do {
        // fs::path p2 = fs::relative(p1, fs::current_path());
        fs::path p2 = fs::relative(p1, "../../abc");
        cout << p2 << endl;
    } while (0);

    // fs::path pp(p1.parent_path());
    // pp /= p1.filename();
    // cout << pp << endl;
}

void test_dir()
{
    namespace fs = boost::filesystem;
    fs::path p1 = fs::current_path();
    cout << p1.stem().string() << endl;
    cout << p1.filename() << endl; // 最后一级目录
    cout << fs::is_directory(p1) << endl;
}

void test_mkdir()
{
    namespace fs = boost::filesystem;
    try {
        fs::path p1("/tmp/a/b/c/d");
        // cout << fs::is_empty(p1) << endl;  // exception 不可以用于不存在的目录
        if (fs::create_directories("/tmp/a/b/c/d"))
            cout << "created dir " << p1 << endl;
        else
            cout << "create dir fail!" << endl;
        cout << fs::is_empty(p1) << endl;
    } catch (const std::exception &ex) {
        cout << ex.what() << endl;
    }
}

void test_rmdir()
{
    namespace fs = boost::filesystem;
    try {
        fs::path p1("/tmp/a");
        cout << fs::remove_all(p1) << endl;
    } catch (const std::exception &ex) {
        cout << ex.what() << endl;
    }
}


int main(int argc, char* argv[])
{
    // google::InitGoogleLogging(argv[0]);

    try {
        // test1();
        // test_dir();
        test_mkdir();
        // test_rmdir();

    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
    } // try

    return 0;
}
