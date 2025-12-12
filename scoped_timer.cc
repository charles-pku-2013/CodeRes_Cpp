#include <fmt/core.h>
#include <glog/logging.h>

#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <thread>  // 用于模拟耗时操作 std::this_thread::sleep_for
#include <utility>

auto DefaultLogger = [](const std::string& message) {
    std::cerr << message << std::endl;
};

// --- 计时器类定义开始 ---
template <typename Logger = decltype(DefaultLogger)>
struct ScopedTimer {
    explicit ScopedTimer(const std::string& description, Logger logger = DefaultLogger)
        : description_(description),
          logger_(std::forward<Logger>(logger)),  // NOTE here same as std::move()
          start_tp_(std::chrono::high_resolution_clock::now()) {}

    ~ScopedTimer() {
        if (!end_tp_.has_value()) {
            stop();
        }
    }

    void stop() {
        if (end_tp_.has_value()) {
            return;
        }

        end_tp_ = std::chrono::high_resolution_clock::now();
        elapsed_ = end_tp_.value() - start_tp_;

        std::string message =
            fmt::format("{} time cost {} seconds", description_, elapsed_.value().count());
        logger_(message);
    }

    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;
    ScopedTimer(ScopedTimer&&) = delete;
    ScopedTimer& operator=(ScopedTimer&&) = delete;

    std::string                                                   description_;
    Logger                                                        logger_;
    std::chrono::high_resolution_clock::time_point                start_tp_;
    std::optional<std::chrono::high_resolution_clock::time_point> end_tp_;
    std::optional<std::chrono::duration<double>>                  elapsed_;
};

// --- 计时器类定义结束 ---

// --- 测试函数 ---

void simulate_work(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// 示例 1: 使用默认 DLOG 的自动 RAII 计时
void test_auto_raii_timer() {
    // 使用默认 Logger（DefaultLogger），离开作用域自动停止
    ScopedTimer timer("Test Auto RAII Function");

    std::cout << "Starting auto RAII test work...\n";
    simulate_work(250);
    std::cout << "Finishing auto RAII test work.\n";

    // 当函数结束时，计时器自动析构并打印日志
}

// 示例 2: 使用自定义 std::cout Logger 的手动停止计时
void test_manual_stop_timer() {
    // 定义一个 Lambda 作为自定义 Logger
    auto custom_cout_logger = [](const std::string& msg) {
        // 使用 std::cerr 区分输出
        std::cerr << "[STDERR CUSTOM LOG] " << msg << std::endl;
    };

    // 实例化计时器，指定模板类型和构造函数参数
    ScopedTimer<decltype(custom_cout_logger)> timer("Test Manual Stop Function",
                                                    custom_cout_logger);

    std::cout << "Starting manual stop test work (part 1)...\n";
    simulate_work(150);

    // 手动停止计时，在这里会立即打印日志
    timer.stop();

    std::cout << "Work stopped and logged. Doing extra work now (not timed).\n";
    simulate_work(100);

    // 当函数结束时，析构函数会被调用，但因为已经 stop() 过了，不会重复打印。
}

int main() {
    std::cout << "--- Starting Timer Tests ---\n\n";

    std::cout << "--- Test Case 1: Auto RAII ---\n";
    test_auto_raii_timer();

    std::cout << "\n--- Test Case 2: Manual Stop with Custom Logger ---\n";
    test_manual_stop_timer();

    std::cout << "\n--- All Tests Finished ---\n";

    return 0;
}
