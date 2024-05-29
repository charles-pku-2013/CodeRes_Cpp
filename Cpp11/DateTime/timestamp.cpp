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

void gettimeofday_test() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    std::tm tm;
    localtime_r(&tv.tv_sec, &tm);
    char buffer[64] = {'\0'};
    std::size_t n = std::strftime(buffer, 64, "%Y%m%d %H:%M:%S", &tm);
    snprintf(buffer + n, sizeof(buffer) - n, "_%06ld", tv.tv_usec);
    cout << buffer << endl;
}

// 精确到微秒的时间戳 20220315_113345_123456
std::string gen_timestamp() {
    char buf[64] = {'\0'};
    struct timeval tv;
    gettimeofday(&tv, NULL);
    std::tm tm;
    localtime_r(&tv.tv_sec, &tm);
    std::size_t n = std::strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tm);
    snprintf(buf + n, sizeof(buf) - n, "_%06ld", tv.tv_usec);
    return buf;
}

// time_t to tm and then strftime
void demo1() {
    std::time_t now = std::time(0);
    std::tm tm;
    localtime_r(&now, &tm);
    char buffer[64] = {'\0'};
    std::strftime(buffer, 64, "%Y%m%d%H%M%S", &tm);  // 20210604174302
    cout << buffer << endl;
}

int main() {
    std::time_t result = std::time(nullptr);
    std::cout << std::ctime(&result) << std::endl;
    // const std::chrono::zoned_time cur_time{ std::chrono::current_zone(),
                                            // std::chrono::system_clock::now() };
    // std::cout << cur_time << '\n';
    std::cout << std::chrono::system_clock::now() << std::endl;
    return 0;

    demo1();
    cout << std::chrono::system_clock::now() << endl;  // Fri Jun  4 17:43:02 2021
    gettimeofday_test();  // 20210604 17:43:02_156937
    SLEEP_MILLISECONDS(2);
    gettimeofday_test();

    return 0;
}

#if 0
namespace example1 {
bool TFSPredictor::_SaveErrReq(const Request &req, const std::string &err_msg) {
    if (!save_err_req_) { return false; }

    try {
        char buf[64] = {'\0'};
        struct timeval tv;
        gettimeofday(&tv, NULL);
        std::tm tm;
        localtime_r(&tv.tv_sec, &tm);
        std::size_t n = std::strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tm);
        snprintf(buf + n, sizeof(buf) - n, "_%06ld", tv.tv_usec);

        // save binary req data
        if (!save_req(&req, err_req_dir_ + "/" + buf + ".dat")) {
            LOG(ERROR) << DebugString() << " save request fail!";
            return false;
        }

        // save err msg
        std::ofstream ofs(err_req_dir_ + "/" + buf + ".txt");
        if (!ofs) {
            LOG(ERROR) << DebugString() << " save error msg fail!";
            return false;
        }
        ofs << err_msg << std::endl;

        return true;
    } catch (const std::exception &ex) {
        LOG(ERROR) << DebugString() << " _SaveErrReq error: " << ex.what();
        return false;
    } catch (...) {
        LOG(ERROR) << DebugString() << " _SaveErrReq unknown error!";
        return false;
    }
}

template<typename ProtoType>
inline bool save_req(const ProtoType *msg, const std::string &fname) {
    std::ofstream ofs(fname, std::ios::out | std::ios::binary);
    if (!ofs) { return false; }
    return msg->SerializeToOstream(&ofs);
}
}  // namespace example1
#endif

