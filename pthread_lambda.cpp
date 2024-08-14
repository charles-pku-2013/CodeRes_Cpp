#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

int main()
{
    pthread_t tid = nullptr;

    // NOTE!!! lambda must has no capture
    auto thread_fn = [](void*)->void* {
        std::cout << "IN THREAD FN" << std::endl;
        ::sleep(5);
        return nullptr;
    };

    void *res;
    int s;

    pthread_create(&tid, nullptr, thread_fn, nullptr);

    std::cout << "Waiting thread ..." << std::endl;
    pthread_join(tid, nullptr);
    std::cout << "end" << std::endl;

    return 0;
}
