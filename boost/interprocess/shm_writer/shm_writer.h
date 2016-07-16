#ifndef _SHM_WRITER_H_
#define _SHM_WRITER_H_

#include "common_utils.h"
#include "stream_buf.h"
#include <string>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

namespace BigRLab {

class ShmWriter : public Writer {
    struct shm_remover {
        shm_remover() { boost::interprocess::shared_memory_object::remove(SHM_NAME); /* DLOG(INFO) << "shm_remover constructor"; */ }
        ~shm_remover() { boost::interprocess::shared_memory_object::remove(SHM_NAME); /* DLOG(INFO) << "shm_remover destructor"; */ }
    };

public:
    ShmWriter()
    { init_shm();}

    virtual bool readLine( std::string &line );
    virtual void writeLine( const std::string &msg );

private:
    void init_shm();
    void resetStream();

private:
    boost::shared_ptr<shm_remover>                                pShmRemover;
    boost::shared_ptr<boost::interprocess::managed_shared_memory> pShmSegment;
    boost::shared_ptr<StreamBuf>                                  pStreamBuf;
    boost::shared_ptr<boost::interprocess::bufferstream>          pStream;
};

} // namespace BigRLab


#endif

