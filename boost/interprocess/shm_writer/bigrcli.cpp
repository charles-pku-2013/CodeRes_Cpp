#include "stream_buf.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <climits>
#include <gflags/gflags.h>

#define MAX_TIMEOUT         2592000   // 1 month

#define RETVAL(val, args) \
    do { \
        std::stringstream __err_stream; \
        __err_stream << args; \
        __err_stream.flush(); \
        std::cerr << __err_stream.str() << std::endl; \
        return val; \
    } while (0)

DEFINE_int32(timeout, MAX_TIMEOUT, "Timeout in seconds for waiting response.");

using namespace boost::interprocess;
using namespace std;

namespace {

static bool validate_timeout(const char* flagname, gflags::int32 value) 
{ 
    if (FLAGS_timeout <= 0)
        FLAGS_timeout = MAX_TIMEOUT;
    return true;
}
static const bool timeout_dummy = gflags::RegisterFlagValidator(&FLAGS_timeout, &validate_timeout);

} // namespace


int main(int argc, char **argv)
try {
    int idx = gflags::ParseCommandLineFlags(&argc, &argv, true);
    // cout << "FLAGS_timeout = " << FLAGS_timeout << endl;
    // for (; idx < argc; ++idx)
        // cout << argv[idx] << " ";
    // cout << endl;
    // return 0;

    StreamBuf *pBuf = NULL;
    std::shared_ptr<managed_shared_memory>   pShmSegment;

    try {
        pShmSegment = std::make_shared<managed_shared_memory>(open_only, SHM_NAME);
        auto ret = pShmSegment->find<StreamBuf>("StreamBuf");  // return pair<type*, size_t>
        pBuf = ret.first;
        if (!pBuf)
            RETVAL(-1, "Cannot load shared buffer object. "
                    "Please make sure that BigRLab apiserver has benn launched with -b option");

    } catch (const std::exception &ex) {
        RETVAL(-1, "Cannot open shared memory object. "
                "Please make sure that BigRLab apiserver has benn launched with -b option");
    } // try

    string cmd, resp;

    if (idx >= argc) {
        cmd = "hello";
    } else {
        for (; idx < argc; ++idx)
            cmd.append(argv[idx]).append(" ");
        cmd.erase(cmd.size()-1);
    } // if

    bufferstream stream(pBuf->buf, SHARED_STREAM_BUF_SIZE);

    auto resetStream = [&] {
        stream.clear();
        stream.seekg(0, std::ios::beg);
        stream.seekp(0, std::ios::beg);
    };

    // clear state
    pBuf->respReady = false;

    // send cmd
    {
        resetStream();
        pBuf->clear();
        scoped_lock<interprocess_mutex> lk(pBuf->mtx);
        stream << cmd << endl << flush;
        pBuf->reqReady = true;
        lk.unlock();
        pBuf->condReq.notify_all();
    }

    // waiting for server response
    {
        resetStream();
        //!! 不能用local_time
        auto deadline = boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(FLAGS_timeout);
        scoped_lock<interprocess_mutex> lk(pBuf->mtx);
        if (!pBuf->condResp.timed_wait(lk, deadline, [&]{ return pBuf->respReady; }))
            RETVAL(-1, "Wait cmd response timeout. "
                    "Please make sure that BigRLab apiserver has benn launched with -b option");
        getline(stream, resp, '\0');
        pBuf->respReady = false;
        lk.unlock();
        boost::trim(resp);
        if (!resp.empty())
            cout << resp << endl;
    }

    return 0;

} catch (const std::exception &ex) {
    cerr << "Exception caught by main: " << ex.what() << endl;
    exit(-1);
}
