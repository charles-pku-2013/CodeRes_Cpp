#include "shared_buffer.h"
#include "wrapunix.h"

int main()
{
    int         fd;
    void*       ptr = NULL;
    const size_t    memLen = sizeof(SharedBuffer) + SHARED_BUFSIZ;
    // const char *sharedMemPath = Px_ipc_name( SHARED_NAME );
    // DBG("sharedMemPath = %s", sharedMemPath);

    //!! 必须先启动producer，再启动consumer
    shm_unlink( SHARED_NAME );       /* OK if this fails */
    fd = Shm_open( SHARED_NAME, O_RDWR | O_CREAT | O_EXCL, FILE_MODE);
    ptr = Mmap(NULL, memLen, PROT_READ | PROT_WRITE,
               MAP_SHARED, fd, 0);
    Ftruncate(fd, memLen);
    Close(fd);

    pBaseAddr = (char*)ptr + sizeof(SharedBuffer);
    SharedBuffer *pSharedBuf = new (ptr) SharedBuffer( SHARED_BUFSIZ ); 

    unsigned char i = 0;
    while( true ) {
        ++i;
        pSharedBuf->Write( &i, 1 );
        DBG("Produced %02u", i);
        sleep(1);
    } // while 

    return 0;
}
