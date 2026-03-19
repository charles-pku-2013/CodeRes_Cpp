#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

// ==========================================
// 模拟自定义的异步任务
// ==========================================
void start_custom_worker(boost::asio::io_context& ioc, std::string task_name, int work_time_ms) {
    // 启动后台线程模拟耗时工作（可以是设备轮询、复杂计算等）
    std::thread([&ioc, task_name, work_time_ms]() {
        // 1. 执行耗时任务
        std::this_thread::sleep_for(std::chrono::milliseconds(work_time_ms));

        // 2. 任务完成！将结果作为事件投递到主事件循环中
        // boost::asio::post 是绝对线程安全的，它可以随时从任何线程把事件塞入 ioc
        boost::asio::post(ioc, [task_name]() {
            std::cout << ">>> [事件就绪] " << task_name << " 已完成，正在处理...\n";
            // 这里可以写具体的处理逻辑
        });
    }).detach();
}

int main() {
    std::cout << "--- 启动长期运行的事件驱动服务 ---\n";

    boost::asio::io_context ioc;

    // 1. 【核心】使用 work_guard 防止 ioc.run() 在无任务时退出
    // 只要 work_guard 存在，ioc 就会一直阻塞等待新事件
    auto work_guard = boost::asio::make_work_guard(ioc);

    // 2. 【核心】注册系统信号处理 (Ctrl+C = SIGINT)
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
        if (!error) {
            std::cout << "\n[系统] 捕获到中断信号 (" << signal_number << ")，准备安全退出...\n";
            // 停止 io_context，这会让阻塞的 ioc.run() 返回
            ioc.stop();
        }
    });

    // 3. 初始投递几个任务
    start_custom_worker(ioc, "Task_A (初始, 2秒)", 2000);
    start_custom_worker(ioc, "Task_B (初始, 1秒)", 1000);

    // 4. 模拟在程序运行期间，不断有"新的等待对象"加入
    std::thread([&ioc]() {
        for (int i = 1; i <= 3; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::string name = "Task_Dynamic_" + std::to_string(i) + " (动态添加)";
            std::cout << "[系统] 动态注入新任务: " << name << "\n";
            start_custom_worker(ioc, name, 1500);  // 1.5秒后就绪
        }
    }).detach();

    std::cout << "主线程进入 Event Loop (按 Ctrl+C 退出)...\n";

    // 5. 启动事件循环。它会一直阻塞在这里处理事件，直到 ioc.stop() 被调用
    ioc.run();

    std::cout << "--- 事件循环已终止，程序安全退出 ---\n";
    return 0;
}
