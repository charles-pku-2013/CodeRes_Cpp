#include <iostream>
#include <sstream>
#include <locale>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

inline
bool str2time(const std::string &s, std::tm &tm,
              const std::string &format = "%Y-%m-%d %H:%M:%S",
              const std::string &encoding = "")
{
    std::istringstream ss(s);
    if (!encoding.empty())
        ss.imbue(std::locale(encoding));
    ss >> std::get_time(&tm, format.c_str());
    return !(ss.fail() || ss.bad());
}


inline
bool str2time(const std::string &s, std::time_t &t,
              const std::string &format = "%Y-%m-%d %H:%M:%S",
              const std::string &encoding = "")
{
    std::tm tm = {};
    bool ret = str2time(s, tm, format, encoding);
    if (!ret) return false;
    t = std::mktime(&tm);
    return true;
}


inline
bool str2time(const std::string &s,
              std::chrono::system_clock::time_point &tp,
              const std::string &format = "%Y-%m-%d %H:%M:%S",
              const std::string &encoding = "")
{
    std::time_t t = 0;
    bool ret = str2time(s, t, format, encoding);
    if (!ret) return false;
    tp = std::chrono::system_clock::from_time_t(t);
    return true;
}


inline
std::string time2str(const std::tm &tm,
                     const std::string &format = "%Y-%m-%d %H:%M:%S",
                     const std::string &encoding = "")
{
    std::ostringstream ss;
    if (!encoding.empty())
        ss.imbue(std::locale(encoding));
    ss << std::put_time(&tm, format.c_str()) << std::flush;
    return ss.str();
}


inline
std::string time2str(const std::time_t &t,
                     const std::string &format = "%Y-%m-%d %H:%M:%S",
                     const std::string &encoding = "")
{
    std::tm tm = *std::localtime(&t);
    return time2str(tm, format, encoding);
}


inline
std::string time2str(const std::chrono::system_clock::time_point &tp,
                     const std::string &format = "%Y-%m-%d %H:%M:%S",
                     const std::string &encoding = "")
{
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    return time2str(t, format, encoding);
}


static
void test4()
{
    std::time_t now = std::time(0);
    cout << time2str(now) << endl;  // 2021-06-04 17:49:09
}


static
void test5()
{
    auto now = std::chrono::system_clock::now();
    cout << time2str(now) << endl;  // 2021-06-04 17:48:18
}


static
void test3()
{
    std::chrono::system_clock::time_point   future;
    bool success = str2time("2016-10-31", future);
    if (!success) {
        cerr << "parse fail!" << endl;
        return;
    } // if

    auto now = std::chrono::system_clock::now();
    auto duration = future - now;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    cout << hours.count() << endl;
}


static
void test1()
{
    std::tm tm = {};
    // bool success = str2time("2016-10-1 11:30:45", tm);
    bool success = str2time("2016-10-1", tm);
    if (success) {
        cout << std::put_time(&tm, "%c") << endl;  // Sun Oct  1 00:00:00 2016
    } else {
        cout << "Parse fail!" << endl;
    } // if
}

static
void test2()
{
    std::time_t t;
    str2time("2016-10-31", t);
    std::time_t now = time(0);
    cout << (t - now) / 3600 << endl;
    cout << ctime(&t) << endl;
}

static
void test6()
{
    const char *str = "20160518";   // 紧凑型
    std::time_t tm = 0;
    bool ret = str2time(str, tm, "%Y%m%d");
    cout << ret << endl;
    cout << time2str(tm) << endl;  // 2016-05-18 00:00:00
}

void test7() {
    std::string format = "%H:%M:%S";
    std::string s("12:30:00abc");
    std::istringstream ss(s);
    std::tm tm{};
    ss >> std::get_time(&tm, format.c_str());
    if (ss.fail()) {
        std::cerr << "Bad string" << std::endl;
    }
    std::cerr << tm.tm_hour << std::endl;
    std::cerr << tm.tm_min << std::endl;
    std::cerr << tm.tm_sec << std::endl;
    std::cerr << tm.tm_year << std::endl;
    std::cerr << tm.tm_mon << std::endl;
    std::cerr << tm.tm_mday << std::endl;
}

int main()
{
    // test1();
    // test2();
    // test3();
    // test4();
    // test5();
    // test6();
    test7();

    return 0;
}

#if 0
int main()
{
    std::tm t = {};
    std::istringstream ss("2011-二月-18 23:12:34");
    ss.imbue(std::locale("zh_CN.utf-8"));
    ss >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");

    if (ss.fail()) {
        std::cout << "Parse failed\n";
    } else {
        std::cout << std::put_time(&t, "%c") << '\n';
    } // if

    time_t old = std::mktime(&t);
    time_t now = time(0);
    cout << (now - old) / 3600 / 24 / 365 << endl;

    // auto tp = std::chrono::system_clock::from_time_t(std::mktime(&t));
    // auto now = std::chrono::system_clock::now();
    // auto duration = now - tp;
    // auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    // cout << duration.count() << endl;
    // cout << duration.count() / 24 / 365 << endl;

    return 0;
}
#endif

