#include "stream_buf.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
#define TIMEOUT         5000

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
    string strResp;
    while (true) {
        SLEEP_MILLISECONDS(100);

        // Clear errors and rewind
        // 每次从头开始写，否则缓存很快用光
        stream.clear();
        stream.seekp(0, std::ios::beg);

        // send msg to server
        scoped_lock<interprocess_mutex> lk(pBuf->mtx);
        stream << "Client send msg " << ++count << endl << flush;
        pBuf->reqReady = true;
        pBuf->condReq.notify_all();

        // waiting for server response
        stream.clear();
        stream.seekg(0, std::ios::beg);
        // pBuf->condResp.wait(lk, [&]{ return pBuf->respReady; });
        //!! 不能用local_time
        auto deadline = boost::posix_time::microsec_clock::universal_time() + boost::posix_time::milliseconds(TIMEOUT);
        if (!pBuf->condResp.timed_wait(lk, deadline, [&]{ return pBuf->respReady; })) {
            cerr << "Wait server response timeout!" << endl;
            return 0;
        } // if timeout
        // pBuf->condResp.wait(lk, [&]{ return pBuf->respReady; });
        while (getline(stream, strResp))
            cout << strResp << endl;
        pBuf->respReady = false;
        lk.unlock();
    } // while

    return 0;

} catch (const std::exception &ex) {
    cerr << "Exception caught by main: " << ex.what() << endl;
}
