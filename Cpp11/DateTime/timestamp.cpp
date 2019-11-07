#include <ctime>
#include <chrono>
#include <string>
#include <iostream>
#include <thread>
#include <sys/time.h>

#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
#define SLEEP_MICROSECONDS(x) std::this_thread::sleep_for(std::chrono::microseconds(x))

using namespace std;

// ctime using time_t
template<typename Clock, typename Duration>
std::ostream &operator<<(std::ostream &stream,
            const std::chrono::time_point<Clock, Duration> &time_point) {
    const time_t time = Clock::to_time_t(time_point);  // time_point to time_t
    // Maybe the put_time will be implemented later?
    std::tm tm;
    localtime_r(&time, &tm);
    char buffer[64] = {'\0'};
    ctime_r(&time, buffer);
    buffer[std::strlen(buffer) - 1] = '\0';  // remove newline char
    return stream << buffer;
}

// 精确到微秒的时间戳
void gettimeofday_test() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    std::tm tm;
    localtime_r(&tv.tv_sec, &tm);
    char buffer[64] = {'\0'};
    std::size_t n = std::strftime(buffer, 64, "%Y%m%d %H:%M:%S", &tm);
    snprintf(buf + n, sizeof(buf) - n, "_%06ld", tv.tv_usec);
    cout << buffer << endl;
}

// time_t to tm and then strftime
void demo1() {
    std::time_t now = std::time(0);
    std::tm tm;
    localtime_r(&now, &tm);
    char buffer[64] = {'\0'};
    std::strftime(buffer, 64, "%Y%m%d%H%M%S", &tm);
    cout << buffer << endl;
}

int main() {
    demo1();
    cout << std::chrono::system_clock::now() << endl;
    gettimeofday_test();
    SLEEP_MILLISECONDS(2);
    gettimeofday_test();

    return 0;
}

