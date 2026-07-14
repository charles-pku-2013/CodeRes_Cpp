#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 1. 定义并初始化控制变量（必须使用 PTHREAD_ONCE_INIT）
pthread_once_t once_control = PTHREAD_ONCE_INIT;

// 2. 需要确保只执行一次的初始化函数
void init_shared_resource(void) {
    printf("[初始化] 正在初始化全局资源（这行字应该只打印一次）...\n");
    sleep(1); // 模拟耗时的初始化操作
    printf("[初始化] 全局资源初始化完成！\n");
}

// 3. 线程的工作函数
void* thread_worker(void* arg) {
    long thread_id = (long)arg;
    printf("线程 %ld: 启动，准备调用 pthread_once\n", thread_id);

    // 所有线程都会执行到这里，但 pthread_once 会确保 init_shared_resource 只运行一次
    pthread_once(&once_control, init_shared_resource);

    printf("线程 %ld: 跨过了 pthread_once，继续后续工作...\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[3];

    // 创建 3 个并发线程
    for (long i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_worker, (void*)i);
    }

    // 等待所有线程结束
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("主线程: 所有线程执行完毕。\n");
    return 0;
}

#if 0
线程 0: 启动，准备调用 pthread_once
[初始化] 正在初始化全局资源（这行字应该只打印一次）...
线程 1: 启动，准备调用 pthread_once
线程 2: 启动，准备调用 pthread_once
[初始化] 全局资源初始化完成！
线程 0: 跨过了 pthread_once，继续后续工作...
线程 2: 跨过了 pthread_once，继续后续工作...
#endif
