/*
c++ -o /tmp/test google-glog/glog_fmt_color.cpp -lglog -lgflags -lfmt -std=c++17 -DGLOG_USE_GLOG_EXPORT -g
 */
/*
 * NOTE 如果将输出重定向到文件用`less -R`查看
 * Use 'AnsiEsc' plugin in vim
 * :%s/\e\[[0-9;]*[mGK]//g
 *
 * 用sed命令处理文件
 * sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2})?)?[mGK]//g" your_log_file.log
 */
#include <glog/logging.h>
#include <fmt/color.h>
#include <fmt/chrono.h> // 用于处理时间格式化
#include <iostream>
#include <unistd.h>

#define ERR_MSG(msg_format, ...) \
    fmt::format(fmt::fg(fmt::color::red) | fmt::emphasis::bold, msg_format, ##__VA_ARGS__)
#define WARN_MSG(msg_format, ...) \
    fmt::format(fmt::fg(fmt::color::yellow) | fmt::emphasis::bold, msg_format, ##__VA_ARGS__)

// 自动判断是否是输出终端
inline bool enable_color() {
    static bool supports_color = isatty(fileno(stderr));
    return supports_color;
}

#define SMART_ERR_MSG(msg_format, ...) \
    (enable_color() ? \
    fmt::format(fmt::fg(fmt::color::red) | fmt::emphasis::bold, msg_format, ##__VA_ARGS__) : \
    fmt::format(msg_format, ##__VA_ARGS__))

// simple
int main() {
    google::InitGoogleLogging("test");
    FLAGS_logtostderr = 1;

    LOG(ERROR) << SMART_ERR_MSG("Hello world {}", "test");

    // 使用 fmt::format 生成彩色字符串再交给 glog
    LOG(INFO) << fmt::format(fg(fmt::color::cyan), "这是一条蓝青色的日志消息");
    LOG(ERROR) << fmt::format(fg(fmt::color::red) | fmt::emphasis::bold, "严重的错误提示！");
    LOG(INFO) << fmt::format(fg(fmt::color::sea_green), "Hello, Green World!");
    LOG(INFO) << fmt::format(fg(fmt::rgb(255, 165, 0)) | fmt::emphasis::underline, "Custom Orange Text");
    LOG(INFO) << fmt::format(fg(fmt::color::steel_blue) | bg(fmt::color::white) | fmt::emphasis::bold | fmt::emphasis::underline,
               "Bold Blue on White");
    LOG(ERROR) << ERR_MSG("Error message test {}", 10);

    std::string msg = ERR_MSG("Error message test {}", 20);
    std::cerr << msg << std::endl;
    LOG(INFO) << fmt::format(bg(fmt::color::cyan), "{}", msg);  // fg(fmt::color::cyan) 不会覆盖原来的红色
    LOG(ERROR) << ERR_MSG(msg);
    LOG(WARNING) << WARN_MSG("Warning, some error happening!");

    return 0;
}

#if 0
class ColorSink : public google::LogSink {
public:
    void send(google::LogSeverity severity, const char* full_filename,
              const char* base_filename, int line,
              const struct ::tm* tm_time,
              const char* message, size_t message_len) override {

        // 1. 使用 memory_buffer 减少内存分配
        fmt::memory_buffer buf;

        // 2. 根据日志级别定义样式
        fmt::text_style style;
        char severity_char = 'I';
        switch (severity) {
            case google::GLOG_WARNING:
                style = fg(fmt::color::yellow);
                severity_char = 'W';
                break;
            case google::GLOG_ERROR:
                style = fg(fmt::color::red) | fmt::emphasis::bold;
                severity_char = 'E';
                break;
            case google::GLOG_FATAL:
                style = bg(fmt::color::red) | fg(fmt::color::white) | fmt::emphasis::bold;
                severity_char = 'F';
                break;
            default:
                style = fg(fmt::color::white);
                severity_char = 'I';
                break;
        }

        // 3. 手动格式化日志前缀（效仿 glog 默认格式：[I/W/E/F]MMDD HH:MM:SS.uuuuuu threadid file:line]）
        // 这里简化了微秒和线程 ID，可根据需要通过 <chrono> 或 getpid 扩展
        fmt::format_to(std::back_inserter(buf),
            "{}{:02d}{:02d} {:02d}:{:02d}:{:02d}] {}:{} ",
            severity_char, tm_time->tm_mon + 1, tm_time->tm_mday,
            tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec,
            base_filename, line);

        // 4. 追加原始消息正文
        fmt::format_to(std::back_inserter(buf), "{}\n", std::string_view(message, message_len));

        // 5. 使用带样式的 fmt::print 输出缓冲区内容
        // buf.data() 获取指针，buf.size() 获取长度，避免多余的 string 拷贝
        fmt::print(style, "{}", fmt::to_string(buf));
    }
};

int main(int argc, char* argv[]) {
    // 强制输出到标准错误流，方便查看颜色
    FLAGS_logtostderr = 0;
    FLAGS_alsologtostderr = false;

    google::InitGoogleLogging(argv[0]);

    ColorSink color_sink;
    google::AddLogSink(&color_sink);

    LOG(INFO) << "这是一条通过 memory_buffer 格式化的普通日志";
    LOG(WARNING) << "这条警告日志是黄色的";
    LOG(ERROR) << "这条错误日志是红色加粗的";

    google::RemoveLogSink(&color_sink);
    return 0;
}
#endif

#if 0
class ColorSink : public google::LogSink {
public:
    void send(google::LogSeverity severity, const char* full_filename,
              const char* base_filename, int line,
              const struct ::tm* tm_time,
              const char* message, size_t message_len) override {

        // 根据级别选择颜色
        fmt::text_style style;
        switch (severity) {
            case google::GLOG_WARNING: style = fg(fmt::color::yellow); break;
            case google::GLOG_ERROR:   style = fg(fmt::color::red) | fmt::emphasis::bold; break;
            case google::GLOG_FATAL:   style = bg(fmt::color::red) | fg(fmt::color::white); break;
            default:                   style = fg(fmt::color::white); break;
        }

        // 使用 ToString() 获取完整格式化后的日志行（含前缀）
        std::string full_msg = ToString(severity, base_filename, line, tm_time, message, message_len);
        fmt::print(style, "{}\n", full_msg);
    }
};

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);

    ColorSink color_sink;
    google::AddLogSink(&color_sink); // 注册自定义 Sink
    FLAGS_alsologtostderr = false;   // 禁用默认输出，避免重复

    LOG(INFO) << "普通信息";
    LOG(WARNING) << "警告信息";
    LOG(ERROR) << "错误信息";

    google::RemoveLogSink(&color_sink);
    return 0;
}
#endif
