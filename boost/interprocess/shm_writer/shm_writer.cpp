#include "shm_writer.h"
#include <iostream>
#include <glog/logging.h>


// for interprocess interact
namespace BigRLab {

using namespace boost::interprocess;
using namespace std;

void ShmWriter::init_shm()
{
    pShmRemover.reset( new shm_remover );
    pShmSegment = boost::make_shared<managed_shared_memory>(create_only, SHM_NAME, SHARED_BUF_SIZE);
    pStreamBuf.reset(pShmSegment->construct<StreamBuf>("StreamBuf")(),
            [&](StreamBuf *p){ if (p) pShmSegment->destroy_ptr(p); });
    pStream = boost::make_shared<bufferstream>(pStreamBuf->buf, SHARED_STREAM_BUF_SIZE);
}

void ShmWriter::resetStream()
{
    pStream->clear();
    pStream->seekg(0, std::ios::beg);
    pStream->seekp(0, std::ios::beg);
}

bool ShmWriter::readLine(string &line)
{
    bool ret = false;
    resetStream();
    scoped_lock<interprocess_mutex> lk(pStreamBuf->mtx);
    // read msg from client
    pStreamBuf->condReq.wait( lk, [&]{return pStreamBuf->reqReady;} );
    ret = getline(*pStream, line);
    pStreamBuf->reqReady = false;
    DLOG(INFO) << "Received cmd from shared buffer: " << line;
    return ret;
}

void ShmWriter::writeLine(const string &msg)
{
    pStreamBuf->clear();
    resetStream();
    scoped_lock<interprocess_mutex> lk(pStreamBuf->mtx);
    *pStream << msg << endl << flush;
    pStreamBuf->respReady = true;
    lk.unlock();
    pStreamBuf->condResp.notify_all();
}

} // namespace BigRLab

