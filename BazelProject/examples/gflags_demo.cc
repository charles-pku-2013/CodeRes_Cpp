/*
 * Tutorial: https://gflags.github.io/gflags/#validate
 * gflags_demo.cpp
 * /tmp/test -nobig_menu --languages=Chinese,English,Japanese
 */

/*
 * Usage:
 * 不可以出现未定义的选项参数
 * app_containing_foo --nobig_menu -languages="chinese,japanese,korean" ...
 * app_containing_foo --languages="chinese,japanese,korean"
 * app_containing_foo -languages="chinese,japanese,korean"
 * app_containing_foo --languages "chinese,japanese,korean"
 * app_containing_foo -languages "chinese,japanese,korean"
 * app_containing_foo --big_menu
 * app_containing_foo --nobig_menu
 * app_containing_foo --big_menu=true
 * app_containing_foo --big_menu=false
 */

/*
 * types:
 * DEFINE_bool: boolean
 * DEFINE_int32: 32-bit integer
 * DEFINE_int64: 64-bit integer
 * DEFINE_uint64: unsigned 64-bit integer
 * DEFINE_double: double
 * DEFINE_string: C++ string
 */

/*
 * NOTE!!! 关于 -flagfile
 * 命令行和flagfile不存在优先级先后，看参数中先指定谁就先解析谁
 */

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <gflags/gflags.h>

#define SPACES " \t\f\r\v\n"

using namespace std;

// 用于声明定义分离
/*
 * DECLARE_bool(big_menu);
 * DECLARE_string(languages);
 */

DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
DEFINE_string(languages, "english,french,german",
             "comma-separated list of languages to offer in the 'lang' menu");
DEFINE_int32(port, 8888, "What port to listen on");

namespace {
// 验证函数，看来至少要每种类型一个验证
bool ValidatePort(const char* flagname, gflags::int32 value)
{
    cout << "ArgValidator() flagname = " << flagname << " value = " << value << endl;
    if (value > 1024 && value < 32768)   // value is ok
        return true;
    printf("Invalid value for --%s: %d\n", flagname, (int)value);
    return false;
}
// 定义port_dummy为了保证RegisterFlagValidator先于main函数执行
const bool port_dummy = gflags::RegisterFlagValidator(&FLAGS_port, &ValidatePort);
} // namespace


void test1( int argc, char **argv )
{
    /*
     * error: ‘namespace’ definition is not allowed here
     * 看来不能在函数中定义
     */
    // DEFINE_int32(port, 0, "port arg in a function.");
    cout << FLAGS_port << endl;
}


int main( int argc, char **argv )
{
    gflags::AllowCommandLineReparsing();
    int idx = gflags::ParseCommandLineFlags(&argc, &argv, true);

    // 未被gflags识别的参数
    for (; idx < argc; ++idx) {
        cout << argv[idx] << endl;
    }

    cout << FLAGS_big_menu << endl;
    cout << FLAGS_languages << endl;
    cout << FLAGS_port << endl;

    // string cmd = "test -port 1234";
    // vector<char*> opts;
    // char *p = const_cast<char*>(cmd.c_str());
    // for (p = strtok(p, SPACES); p; p = strtok(NULL, SPACES))
        // opts.push_back(p);

    // test1(opts.size(), &opts[0]);

    return 0;
}


#if 0
DEFINE_string(server, "", "server addr ip:port");
// lambda 函数参数检查
namespace {
bool _check_server = gflags::RegisterFlagValidator(&FLAGS_server,
            [](const char* flagname, const std::string& value){
    if (value.empty()) {  //!!! NOTE here cannot use FLAGS_server
        std::cerr << flagname << " not set" << std::endl;
        return false;
    }
    return true;
});
} // namespace
#endif
