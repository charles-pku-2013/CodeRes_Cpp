#include "shared_buffer.h"
#include "wrapunix.h"

int main()
{
    int         fd;
    void*       ptr = NULL;
    const size_t    memLen = sizeof(SharedBuffer) + SHARED_BUFSIZ;

    //!! 注意flag和producer不同
    fd = Shm_open( SHARED_NAME, O_RDWR, FILE_MODE);
    ptr = Mmap(NULL, memLen, PROT_READ | PROT_WRITE,
               MAP_SHARED, fd, 0);
    Ftruncate(fd, memLen);
    Close(fd);

    pBaseAddr = (char*)ptr + sizeof(SharedBuffer);
    SharedBuffer *pSharedBuf = (SharedBuffer*)ptr;

    unsigned char ch;
    while( true ) {
        pSharedBuf->Read( &ch, 1 );
        DBG("Consumed %02u", ch);
        // sleep(5);
    } // while 

    return 0;
}
