// c++ -o test test.cpp -std=c++11 -lrt -pthread -g

#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>
#include <fstream>
#include "LOG.h"
#include "shared_buffer.h"


using namespace std;
using namespace boost::interprocess;

/*
 * static
 * void do_routine( SharedBuffer *pSharedBuf )
 * {
 *     const char                  *p = NULL;
 *     uint32_t                    seqNO;
 *     std::size_t                 frameLen;
 *     std::ofstream               ofs( "test.h265", ios::binary | ios::out );
 * 
 *     while( true ) {
 *         pSharedBuf->pop();
 *         frameLen = pSharedBuf->readBuf().size() - 4;
 *         p = pSharedBuf->readBuf().ptr();
 *         seqNO = *(uint32_t*)p;
 *         p += 4;
 *         DBG_STREAM("read encoded frame seqNO: " << seqNO << " length: " << frameLen);
 *         ofs.write( p, frameLen );
 *         ofs.flush();
 *     }
 * 
 *     return;
 * }
 */

static
void do_routine( SharedBuffer *pSharedBuf )
{
    const char *p = NULL;
    const BytesArray &buf = pSharedBuf->readBuf();

    while( true ) {
        pSharedBuf->pop();
        p = buf.ptr();
        std::cout.write( p, buf.size() );
    }

    return;
}

int main()
{
    //Remove shared memory on construction and destruction
    struct shm_remove
    {
        shm_remove()  { shared_memory_object::remove("Encoder_Server_SharedBuffer"); }
        ~shm_remove() { shared_memory_object::remove("Encoder_Server_SharedBuffer"); }
    } remover;

    try {
        //Create shared memory
        managed_shared_memory segment(create_only,"Encoder_Server_SharedBuffer", ENCODER_SERVER_BUFSIZE);

        //An allocator convertible to any allocator<T, segment_manager_t> type
        void_allocator void_alloc(segment.get_segment_manager());
        // std::cout << sizeof(void_alloc) << std::endl; // 8

        // construct the shared buffer obj in shared memory
        SharedBufferPtr pSharedBuf(segment.construct<SharedBuffer>("SharedBuffer")(void_alloc, SHARED_BUFSIZE, INIT_FRAME_SIZE), 
                std::bind(&managed_shared_memory::destroy_ptr<SharedBuffer>, &segment, std::placeholders::_1));

		int *p = segment.construct<int>("IntTest")(10);

		//DBG("Base addr of shared mem is %lx", (long)(segment.get_address()));
		//DBG("Addr of pSharedBuf is %lx", (long)(pSharedBuf.get()));

        do_routine( pSharedBuf.get() );

    } catch ( const std::exception &ex ) {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        return -1;
    }

    return 0;
}

/*
 * static inline
 * uint32_t gen_timestamp()
 * {
 *     typedef std::chrono::system_clock Clock;
 * 
 *     auto now = Clock::now().time_since_epoch();
 *     auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now);
 * 
 *     return (uint32_t)(milliseconds.count());
 * }
 * 
 * int main()
 * {
 *     uint32_t seqNO, frameSize;
 *     vector<char> buf;
 *     buf.resize( MAX_FRAME_SIZE );
 * 
 *     ofstream dbgfs("test.log");
 * 
 *     while( cin ) {
 *         cin.read( (char*)&seqNO, 4 );
 *         dbgfs << "Read frame " << seqNO << std::flush;
 *         cin.read( (char*)&frameSize, 4 );
 *         dbgfs << " size = " << frameSize << endl << std::flush;
 *         assert( frameSize <= MAX_FRAME_SIZE );
 *         cin.read( &buf[0], frameSize );
 * 
 *         // dbgfs << "Read frame " << seqNO << " size " << frameSize << endl;
 * 
 *         if( cin )
 *             cout.write( &buf[0], frameSize );
 *     }
 * 
 *     // if( !cin ) {
 *         // dbgfs << "Input stream error!" << endl;
 *         // exit(-1);
 *     // }
 * 
 *     return 0;
 * }
 */

