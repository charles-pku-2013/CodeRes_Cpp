/*
c++ -o /tmp/test demo.cpp -lboost_filesystem -lboost_system -labsl_strings -std=c++11 -g
https://www.boost.org/doc/libs/1_65_0/libs/filesystem/doc/reference.html
brew install abseil
brew ls abseil
 */
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <boost/filesystem.hpp>
#include "absl/strings/str_join.h"

using namespace std;
namespace fs = boost::filesystem;

#if 0
# examples
## path.stem()
std::cout << path("/foo/bar.txt").stem(); // outputs "bar"
path p = "foo.bar.baz.tar";
for (; !p.extension().empty(); p = p.stem())
    std::cout << p.extension() << '\n';

## path.filename()
std::cout << path("/foo/bar.txt").filename(); // outputs "bar.txt"
std::cout << path("/foo/bar").filename();     // outputs "bar"
std::cout << path("/foo/bar/").filename();    // outputs "."
std::cout << path("/").filename();            // outputs "/"
std::cout << path(".").filename();            // outputs "."
std::cout << path("..").filename();           // outputs ".."

## path.parent_path()
std::cout << path("/foo/bar.txt").parent_path(); // outputs "/foo"
std::cout << path("/foo/bar").parent_path();     // outputs "/foo"
std::cout << path("/foo/bar/").parent_path();    // outputs "/foo/bar"
std::cout << path("/").parent_path();            // outputs ""
std::cout << path(".").parent_path();            // outputs ""
std::cout << path("..").parent_path();           // outputs ""

#endif

void test1(const char *filepath)
{
    cout << "current_path: " << fs::current_path() << endl;
    // fs::path p1("../data/conf.json");
    // fs::path p1("../../Cpp11/type_traits/is_destructible.cpp");
    fs::path p1(filepath);
    cout << "orig: " << p1 << endl;
    cout << "parent_path: " << p1.parent_path() << endl; // 返回文件所在目录
    cout << "filename: " << p1.filename() << endl;
    cout << "stem: " << p1.stem() << endl; // 主文件名 是 fs::path 转成字符串用 stem().string() 不包含前缀路径
    cout << "extension: " << p1.extension() << endl;  // ".cpp"
    cout << "change extension to dat: " << fs::change_extension(p1, ".dat").string() << endl;
    cout << "basename: " << fs::basename(p1) << endl;  // /foo/bar/test.txt -> test
    cout << "is_absolute: " << p1.is_absolute() << endl;
    cout << "is_relative: " << p1.is_relative() << endl;
    cout << "absolute_path: " << fs::absolute(p1) << endl;
    cout << "absolute_path by system_complete: " << fs::system_complete(p1) << endl;  // like above
    cout << "absolute_path by canonical: " << fs::canonical(p1) << endl;  // require exist, otherwise exception
    cout << "relative_path: " << p1.relative_path() << endl;
    cout << "is_directory: " << fs::is_directory(p1) << endl;
    cout << "is_regular_file: " << fs::is_regular_file(p1) << endl;
    cout << "is_symlink: " << fs::is_symlink(p1) << endl;  // regular file 也可以同时是 sym_link
    cout << "exists: " << fs::exists(p1) << endl;
    cout << "root_path:  " << p1.root_path() << endl;
    cout << "root_name: " << p1.root_name() << endl;
    cout << "file_size: " << fs::file_size(p1) << endl;  // bytes

    do {
        fs::path p2 = fs::relative(p1, fs::current_path());
        // fs::path p2 = fs::relative(p1, "../../abc");
        cout << p2 << endl;
    } while (0);

    // fs::path pp(p1.parent_path());
    // pp /= p1.filename();
    // cout << pp << endl;
}

/*
 * 若是目录，判断是否是空目录
 * 若是文件，返回内容是否是空（0字节）
 * 不可以传入不存在的文件或目录，会跑出异常
 */
void check_empty(const char *filepath) {
    fs::path p(filepath);
    cout << fs::is_empty(p) << endl;
    // is_directory(s)
         // ? directory_iterator(p) == directory_iterator()
         // : file_size(p) == 0;
    // p.empty() 只判断名字是否空字符串
}

void test_dir(const std::string &s)
{
    // fs::path p1 = fs::current_path();
    fs::path p1(s);
    cout << fs::is_directory(p1) << endl;

    // NOTE!!! 注意最后的 /
    /*
     * /foo/bar/ 返回 "."
     * /foo/bar  返回 "bar"
     */
    cout << p1.stem().string() << endl;
    cout << p1.filename() << endl;
}

void test_mkdir()
{
    try {
        fs::path p1("/tmp/a/b/c/d");  // same like mkdir -p
        // cout << fs::is_empty(p1) << endl;  // exception 不可以用于不存在的目录
        if (fs::create_directories(p1)) {
            cout << "created dir " << p1 << endl;
        } else {
            cout << "create dir fail!" << endl;  // fail if already exists
        }
        cout << fs::is_empty(p1) << endl;
    } catch (const std::exception &ex) {
        cout << ex.what() << endl;
    }
}

void test_rmdir()
{
    try {
        fs::path p1("/tmp/a");
        cout << fs::remove_all(p1) << endl;
    } catch (const std::exception &ex) {
        cout << ex.what() << endl;
    }
}

void test_remove_file(const char *path) {
    try {
        fs::path p(path);
        fs::remove(p);  // 也可以用 remove_all 删除单个文件
    } catch (const std::exception &ex) {
        cout << "remove fail: " << ex.what() << endl;
    }
}

void test_compare_equal() {
    fs::path p1("foo/bar.txt");
    fs::path p2("./foo/bar.txt");
    // if (fs::equivalent(p1, p2))  // Operation not permitted: "foo/bar.txt", "./foo/bar.txt"
    // if (p1 == p2)  // fail
    // if (p1.compare(p2) == 0) // fail
    // if (fs::absolute(p1) == fs::absolute(p2)) // fail
    if (fs::system_complete(p1) == fs::system_complete(p2)) {  // fail
        cout << "Equal!" << endl;
    } else {
        cout << "Not equal!" << endl;
    }
}

// path iterator 也是一种容器
void test_split_merge_iterator(const std::string &str) {
    fs::path path(str);
    // for (const auto& elem : path) {
        // cout << elem << endl;
    // }
    // std::vector<fs::path> elems(path.begin(), path.end());  // OK

    // split into string vector
    std::vector<std::string> elems;
    std::transform(path.begin(), path.end(), std::back_inserter(elems),
            [](const fs::path &p)->std::string { return p.string(); });
    cout << absl::StrJoin(elems, ",") << endl;
}

int main(int argc, char* argv[])
{
    // google::InitGoogleLogging(argv[0]);

    try {
        // check_empty(argv[1]);
        // test1(argv[1]);
        // test_dir(argv[1]);
        test_mkdir();
        // test_rmdir();
        // test_remove_file(argv[1]);
        // test_compare_equal();
        // test_split_merge_iterator(argv[1]);

    } catch (const std::exception &ex) {
        cerr << "Exception: " << ex.what() << endl;
    } // try

    return 0;
}
