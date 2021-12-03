#include <iostream>
#include <thread>
#include <unistd.h>

#include "tools/thread_safe_queue.h"

int main() {
    using namespace std;

    tools::ThreadSafeQueue<int> que;

    std::thread producer([&que]{
        for (int i = 1; i <= 10; ++i) {
            que.push(i);
            ::sleep(1);
        }
    });

    std::thread consumer([&que]{
        while (true) {
            int i = que.pop();
            cout << i << endl;
        }
    });

    producer.join();
    consumer.join();

    return 0;
}


