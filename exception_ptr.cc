#include <iostream>
#include <thread>
#include <exception>
#include <stdexcept>

// 全局或共享的异常指针
std::exception_ptr globalExceptionPtr = nullptr;

void threadFunction() {
    try {
        // 模拟业务逻辑
        throw std::runtime_error("来自子线程的恶意告白！");
    } catch (...) {
        // 捕获当前正在处理的异常并存储
        globalExceptionPtr = std::current_exception();
    }
}

int main() {
    std::thread t(threadFunction);
    t.join(); // 等待子线程结束

    // 在主线程中检查是否有异常被存入指针
    if (globalExceptionPtr) {
        try {
            // 重新抛出异常，以便进行处理
            std::rethrow_exception(globalExceptionPtr);
        } catch (const std::exception& e) {
            std::cout << "主线程成功捕获: " << e.what() << std::endl;
        }
    } else {
        std::cout << "子线程运行顺利，没有异常。" << std::endl;
    }

    return 0;
}
