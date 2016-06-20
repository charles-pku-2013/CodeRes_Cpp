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

uint32_t get_number(const string &s)
{
    uint32_t ret = 0;
    string::size_type pos = s.find_last_of(" ");
    if (pos == string::npos)
        return 0;
    sscanf( s.c_str()+pos, "%u", &ret );
    return ret;
}


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
        // Clear errors and rewind
        // 每次从头开始写，否则缓存很快用光
        stream.clear();
        stream.seekg(0, std::ios::beg);

        scoped_lock<interprocess_mutex> lk(pBuf->mtx);

        // read msg from client
        pBuf->condReq.wait( lk, [&]{return pBuf->reqReady;} );
        if (getline(stream, line))
            cout << "Server reads line: " << line << endl;
        else
            cout << "Server reads line fail!" << endl;
        pBuf->reqReady = false;

        // response client
        stream.clear();
        stream.seekp(0, std::ios::beg);
        stream << "Server response msg " << get_number(line) << endl << flush; // 必须加endl 因为都是getline，否则会一直读到0
        pBuf->respReady = true;
        lk.unlock();
        pBuf->condResp.notify_all();
    } // while

    cout << "Reader done!" << endl;

    return 0;

} catch (const std::exception &ex) {
    cerr << "Exception caught by main: " << ex.what() << endl;
    exit(-1);
}
