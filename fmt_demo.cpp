/*
c++ -o /tmp/test test.cc -I/opt/homebrew/Cellar/fmt/11.2.0/include -L/opt/homebrew/Cellar/fmt/11.2.0/lib -lfmt -std=c++17 -g
 */
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <map>
#include <fmt/base.h>
#include <fmt/printf.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <fmt/chrono.h>
#include <fmt/ranges.h>

using namespace std;

/*
 * 打印大括号
 * fmt::format("TimeoutTaskQueue: {{size:{}, capacity:{}, timeout:{}}}",
 *                 queue_.size(), queue_.capacity(), timeout_);
 */

int main(int argc, char **argv) {
    fmt::fprintf(stderr, "%s running...\n", argv[0]);
    // fmt::fprintf(std::cerr, "%s running...\n", argv[0]);
    fmt::print("Hello, world!\n");

    // strformat sprintf
    {
        std::string s1 = fmt::format("The answer is {}.", 42);
        cout << s1 << endl;
        std::string s2 = fmt::format("I'd rather be {1} than {0}.", "right", "happy");
        cout << s2 << endl;
        std::string s3 = fmt::sprintf("The age of %s is %d", s2, 20);
        cout << s3 << endl;
    }

    // join container
    {
        std::vector<int> arr{1,2,3};
        fmt::println(std::cerr,"arr = {}", arr);  // 按默认格式输出
        cout << fmt::to_string(arr) << endl;
        std::string s = fmt::to_string(fmt::join(arr, ":"));
        cout << s << endl;
        std::string s1 = fmt::to_string(fmt::join(arr.begin(), arr.end(), ":"));
        cout << s1 << endl;
        std::string s2 = fmt::to_string(fmt::join(arr.data(), arr.data() + 3, ":"));
        cout << s2 << endl;
    }

    // date time
    {
        auto now = std::chrono::system_clock::now();
        fmt::print("Date and time: {}\n", now);
        fmt::print("Time: {:%H:%M}\n", now);
    }

    {
        std::map<std::string, int> dict{
            {"hello", 10},
            {"charles", 20},
            {"bob", 30}
        };
        fmt::print("{}\n", dict);
    }

    return 0;
}
