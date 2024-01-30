#include <iostream>
#include <sys/time.h>

void test() {
    std::cout << "test" << std::endl;

    auto fn_get_timezone = [](void)->const char* {
        static char buf[8] = {0};
        struct timeval tv;
        struct timezone tz;
        ::gettimeofday(&tv, &tz);
        snprintf(buf, sizeof(buf), "%c%02d%02d",
                tz.tz_minuteswest < 0 ? '+' : '-',
                std::abs(tz.tz_minuteswest) / 60, std::abs(tz.tz_minuteswest) % 60);
        std::cerr << "GET TIME ZONE" << std::endl;
        return buf;
    };
    static const char* str_tz = fn_get_timezone();  // 在第一次调用时执行，c++11保证线程安全

    std::cout << str_tz << std::endl;
}

int main() {
    test();

    struct timeval tv;
    struct timezone tz;
    ::gettimeofday(&tv, &tz);

    std::cout << tv.tv_sec << std::endl;
    std::cout << tv.tv_usec << std::endl;
    std::cout << tz.tz_minuteswest << std::endl;   // -480
    std::cout << tz.tz_dsttime << std::endl;      // 0

    return 0;
}

// 1706064251
// 394708
// -480
// 0


