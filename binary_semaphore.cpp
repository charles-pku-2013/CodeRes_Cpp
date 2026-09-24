#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore>

// 初始计数值设为 0，表示初始处于“无信号/锁定”状态
std::binary_semaphore smphSignal{0};

void workerThread() {
    std::cout << "[Worker 线程] 正在准备数据...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 模拟耗时任务

    std::cout << "[Worker 线程] 数据准备完毕，发出通知信号。\n";
    // release() 将信号量计数值加 1（0 -> 1），并唤醒等待的线程
    // 注意：没有锁的所有权限制，任何线程都可以随时调用 release()
    smphSignal.release();
}

int main() {
    std::cout << "[Main 线程] 启动 Worker 线程...\n";
    std::thread t(workerThread);

    std::cout << "[Main 线程] 开始等待 Worker 线程完成...\n";
    // acquire() 尝试将计数值减 1。当前计数值为 0，因此 Main 线程在此阻塞等待
    smphSignal.acquire();

    std::cout << "[Main 线程] 收到通知，继续执行后续逻辑！\n";

    t.join();
    return 0;
}
