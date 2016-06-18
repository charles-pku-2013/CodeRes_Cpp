/*
 * c++ -o /tmp/reader reader.cpp -lrt -std=c++11 -pthread -g
 */
#include "stream_buf.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

#define SHARED_BUF_SIZE    65536

using namespace boost::interprocess;
using namespace std;

int main()
try {
    struct shm_remove {
        shm_remove() { while (shared_memory_object::remove("MySharedMemory")); }
        ~shm_remove(){ cout << "shm_remove destructor" << endl; while (shared_memory_object::remove("MySharedMemory")); }
    } remover;

    managed_shared_memory segment(create_only,
            "MySharedMemory",
            SHARED_BUF_SIZE);

    std::shared_ptr<StreamBuf> pBuf(segment.construct<StreamBuf>("StreamBuf")(),
            [&](StreamBuf *p){ if (p) segment.destroy_ptr(p); });
    
    // StreamBuf *pBuf = segment.construct<StreamBuf>("StreamBuf")(这里填构造参数);

    // 非正常退出，ctrl-c exit() sh_remover析构是不会被执行的，智能指针也一样。
    // exit(0);
    
    bufferstream stream(pBuf->buf, STREAM_BUF_SIZE);

    string line;
    while (true) {
        scoped_lock<interprocess_mutex> lk(pBuf->lock);
        pBuf->cond.wait( lk, [&]{return pBuf->hasData;} );
        if (getline(stream, line))
            cout << "Reader reads line: " << line << endl;
        else
            cout << "Reader reads line fail!" << endl;
        pBuf->hasData = false;
        lk.unlock();
        // Clear errors and rewind
        // 每次从头开始读, 注意 reader seekg (get) writer seekp (put)
        stream.clear();
        stream.seekg(0, std::ios::beg);
    } // while

    cout << "Reader done!" << endl;

    return 0;

} catch (const std::exception &ex) {
    cerr << "Exception caught by main: " << ex.what() << endl;
    exit(-1);
}
