#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>
#include <fstream>
#include <thread>
#include <cstring>
#include "LOG.h"
#include "shared_buffer.h"

using namespace boost::interprocess;

static
void do_routine( SharedBuffer *pSharedBuf )
{
    char buf[64];
    BytesArray& writeBuf = pSharedBuf->writeBuf();

    for( int i = 0; i < 10; ++i ) {
        sprintf( buf, "test %d\n", i );
        DBG_STREAM(buf);
        writeBuf.clear();
        writeBuf.append( buf, strlen(buf) );
        pSharedBuf->push();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    try {
        SharedBuffer*                                                   pSharedBuf;
        std::unique_ptr<boost::interprocess::managed_shared_memory>     segment;

        segment.reset( new managed_shared_memory(open_only,"Encoder_Server_SharedBuffer") );
        std::pair<SharedBuffer*, std::size_t> ret = segment->find<SharedBuffer>("SharedBuffer");
        pSharedBuf = ret.first;
        if( !pSharedBuf )
            throw std::runtime_error("Cannot find object SharedBuffer in shared memory");

		std::pair<int*, std::size_t> ret1 = segment->find<int>("IntTest");
		int *p = ret1.first;
		if ( p )
			DBG_STREAM(*p);

        do_routine(pSharedBuf);

    } catch ( const std::exception &ex ) {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
