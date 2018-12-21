/*
 * c++ -o /tmp/test shared_lock_demo.cpp -lboost_thread-mt -lboost_system -std=c++11 -g
 * 经测试，写者优先
 */
#include <boost/thread.hpp>
#include <iostream>
#include <thread>
#include <cstdio>

#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

using namespace std;

boost::shared_mutex mtx;

void reader(int id) {
    boost::shared_lock<boost::shared_mutex> lock(mtx);
    printf("reader %d start...\n", id);
    SLEEP_SECONDS(5);
    printf("reader %d done\n", id);
}

void writer(int id) {
    boost::unique_lock<boost::shared_mutex> lock(mtx);
    printf("writer %d start...\n", id);
    SLEEP_SECONDS(5);
    printf("writer %d done\n", id);
}

int main() {
    std::thread readers[8];

    for (int i = 0; i < 4; ++i) {
        readers[i] = std::thread(reader, i + 1);
    }

    std::thread writer1(writer, 1);

    for (int i = 4; i < 8; ++i) {
        readers[i] = std::thread(reader, i + 1);
    }

    for (int i = 0; i < 8; ++i) {
        readers[i].join();
    }
    writer1.join();

    return 0;
}
