#include <folly/io/async/AsyncSignalHandler.h>
#include <folly/io/async/EventBase.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <signal.h>

// ==========================================
// 1. 优雅处理系统信号 (类似 Asio 的 signal_set)
// ==========================================
// Folly 推荐通过继承 AsyncSignalHandler 来处理信号
class GracefulShutdownHandler : public folly::AsyncSignalHandler {
public:
    GracefulShutdownHandler(folly::EventBase* evb) : folly::AsyncSignalHandler(evb), evb_(evb) {
        // 注册需要监听的信号
        registerSignalHandler(SIGINT);   // Ctrl+C
        registerSignalHandler(SIGTERM);  // kill
    }

    // 当收到信号时，Folly 会在 EventBase 所在的线程安全地回调此函数
    void signalReceived(int signum) noexcept override {
        std::cout << "\n[系统] 捕获到中断信号 (" << signum << ")，准备安全退出...\n";

        // 通知 EventBase 尽快退出循环 (类似 Asio 的 ioc.stop())
        evb_->terminateLoopSoon();
    }

private:
    folly::EventBase* evb_;
};

// ==========================================
// 2. 模拟自定义的异步任务投递
// ==========================================
void start_custom_worker(folly::EventBase* evb, std::string task_name, int work_time_ms) {
    // 启动后台线程模拟耗时工作
    std::thread([evb, task_name, work_time_ms]() {
        // 1. 模拟复杂的耗时任务 (磁盘 I/O、外部 API 调用等)
        std::this_thread::sleep_for(std::chrono::milliseconds(work_time_ms));

        // 2. 任务完成！跨线程将事件投递回主事件循环
        // runInEventBaseThread 是 Folly 中极其高效的跨线程调用机制
        // 底层使用无锁队列 + eventfd 唤醒 epoll_wait
        evb->runInEventBaseThread([task_name]() {
            std::cout << ">>> [事件就绪] " << task_name << " 已完成，正在 EventBase 线程处理...\n";
            // 真实的业务处理逻辑写在这里
        });
    }).detach();
}

int main() {
    std::cout << "--- 启动基于 Folly 的事件驱动服务 ---\n";

    // 创建 Folly 的核心事件循环对象
    folly::EventBase evb;

    // 实例化信号处理器，绑定到 EventBase
    GracefulShutdownHandler shutdown_handler(&evb);

    // 初始投递几个任务
    start_custom_worker(&evb, "Task_A (初始, 2秒)", 2000);
    start_custom_worker(&evb, "Task_B (初始, 1秒)", 1000);

    // 模拟在程序运行期间，不断有新的任务加入
    std::thread([&evb]() {
        for (int i = 1; i <= 3; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::string name = "Task_Dynamic_" + std::to_string(i) + " (动态添加)";
            std::cout << "[系统] 动态注入新任务: " << name << "\n";
            start_custom_worker(&evb, name, 1500);
        }
    }).detach();

    std::cout << "主线程进入 Event Loop (按 Ctrl+C 退出)...\n";

    // 启动事件循环
    // loopForever() 会一直阻塞并处理事件。
    // 即便当前没有任何待处理的事件，它也不会退出，直到调用 terminateLoopSoon()。
    evb.loopForever();

    std::cout << "--- 事件循环已终止，程序安全退出 ---\n";
    return 0;
}
