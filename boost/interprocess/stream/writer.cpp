/*
 * c++ -o /tmp/writer writer.cpp -lrt -std=c++11 -pthread -g
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

using namespace boost::interprocess;
using namespace std;

int main()
try {
    managed_shared_memory segment( open_only, "MySharedMemory" );

    auto ret = segment.find<StreamBuf>("StreamBuf");  // return pair<type*, size_t>
    StreamBuf *pBuf = ret.first;
    if (!pBuf)
        throw runtime_error("Cannot open shared buffer");

    bufferstream stream(pBuf->buf, STREAM_BUF_SIZE);

    uint32_t count = 0;
    while (true) {
        SLEEP_MILLISECONDS(100);
        scoped_lock<interprocess_mutex> lk(pBuf->lock);
        stream << "Writer send msg " << ++count << endl << flush;
        pBuf->hasData = true;
        lk.unlock();
        pBuf->cond.notify_one();
        // Clear errors and rewind
        // 每次从头开始写，否则缓存很快用光
        stream.clear();
        stream.seekp(0, std::ios::beg);
    } // while

    return 0;

} catch (const std::exception &ex) {
    cerr << "Exception caught by main: " << ex.what() << endl;
}
