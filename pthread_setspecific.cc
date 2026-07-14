#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// 1. 定义一个全局的 Key
pthread_key_t global_tsd_key;

// 析构函数：当线程退出时，如果该线程的私有数据不为 NULL，系统会自动调用此函数释放内存
void destroy_thread_data(void* ptr) {
    printf("[析构] 线程正在退出，自动释放内存地址: %p\n", ptr);
    free(ptr);
}

// 一个模拟的子函数，用来展示无需通过参数传递，就能直接获取线程私有数据
void print_my_private_data() {
    // 3. 使用相同的 Key 获取当前线程特有的数据
    int* my_data = (int*)pthread_getspecific(global_tsd_key);
    printf("子函数读取 -> 当前线程绑定的数据是: %d\n", *my_data);
}

void* thread_worker(void* arg) {
    long thread_id = (long)arg;

    // 2. 为每个线程独立分配一块内存
    int* private_val = malloc(sizeof(int));
    *private_val = (int)(thread_id * 100); // 线程 1 存 100，线程 2 存 200

    printf("线程 %ld: 为自己分配了数据 %d，内存地址: %p\n", thread_id, *private_val, (void*)private_val);

    // 【核心步骤】使用同一个 global_tsd_key，把各自的指针绑定到当前线程
    pthread_setspecific(global_tsd_key, private_val);

    sleep(1); // 故意等待，证明即使交替执行，数据也不会被对方覆盖

    // 调用子函数，内部会去获取各自的数据
    print_my_private_data();

    return NULL;
}

int main() {
    pthread_t t1, t2;

    // 创建全局键，并指定析构函数
    pthread_key_create(&global_tsd_key, destroy_thread_data);

    // 启动两个线程
    pthread_create(&t1, NULL, thread_worker, (void*)1);
    pthread_create(&t2, NULL, thread_worker, (void*)2);

    // 等待线程结束
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // 销毁键
    pthread_key_delete(global_tsd_key);
    printf("主线程: 运行结束。\n");
    return 0;
}

#if 0
线程 1: 为自己分配了数据 100，内存地址: 0x1c4c010
线程 2: 为自己分配了数据 200，内存地址: 0x1c4c030
子函数读取 -> 当前线程绑定的数据是: 100
[析构] 线程正在退出，自动释放内存地址: 0x1c4c010
子函数读取 -> 当前线程绑定的数据是: 200
[析构] 线程正在退出，自动释放内存地址: 0x1c4c030
主线程: 运行结束。
#endif
