#include "zigbee_common.h"

int fd;

void test()
{
	ssize_t n;
	uint8 buf[] = { 0x02, 0x0C, 0, 85, 0, 0, 0x11, 0x11, 0x34, 0x12, 0x06, 0, 
		0x11, 0x01, 0x01, 0xED, 0x03};
	n = writen( fd, buf, sizeof(buf) );
	printf( "write finish %d.", n );
}

int main( int argc, char **argv )
{
    uint8 buf[256], *ptr;
    int ret, i;
    uint16 dstAddr;
    
    if( (fd = open_zigbee_device(argv[1], B115200)) < 0 ) {
        perror( "open serial error" );
        exit(-1);
    }
    
    setbuf( stdout, NULL );
    
    test();
    
    // test read_config
//    buf[0] = 0x83;
//    ret = zigbee_request(0x0426, 1, buf, ZIGBEE_INVALID_CMD, NULL, NULL, ZIGBEE_INVALID_CMD, NULL, NULL );
    
    
END:
    DBG( "read the rest of response..." );
    read_all( fd );
    
    close(fd);
    return 0;
}








