#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
          ((uint32)((uint32)((Byte0) & 0x00FF) \
          + ((uint32)((Byte1) & 0x00FF) << 8) \
          + ((uint32)((Byte2) & 0x00FF) << 16) \
          + ((uint32)((Byte3) & 0x00FF) << 24)))


typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned short uint16;

static struct termios oldtio, newtio;

static int devfd;

// Non-Canonical not text based
int open_serial( const char *dev_name, int baudrate )
{
	int fd;

	fd = open( dev_name, O_RDWR | O_NOCTTY ); 
	if (fd < 0 )
		return fd;

	tcgetattr( fd, &oldtio ); /* save current port settings */

	memset(&newtio, 0, sizeof(newtio));
	newtio.c_cflag = baudrate | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME]	  = 0;
	newtio.c_cc[VMIN]	  = 1; //!! cannot be set to 0, otherwise, read will return immediately with retval 0

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);

	devfd = fd;
	return fd;
}

int close_serial( int fd )
{
	tcsetattr(fd, TCSANOW, &oldtio);
	close(fd);
}

int make_zigbee_frame( uint8 cmd0, uint8 cmd1, 
		uint8 datalen, void *data, void *buf )
{
	void *ptr, *q;
	uint8 tmp;
	
	ptr = buf;

	// write SOF
	tmp = 0xFE;
	memcpy( ptr++, &tmp, 1 );
	// write datalen
	memcpy( ptr++, &datalen, 1 );

	// write cmd
	memcpy( ptr++, &cmd0, 1 );
	memcpy( ptr++, &cmd1, 1 );

	// write data
	if( data ) {
		memcpy( ptr, data, datalen );
		ptr += datalen;
	}

	// compute FCS
	tmp = 0;
	for( q = buf + 1; q != ptr; ++q )
		tmp ^= *(uint8*)q;
	memcpy( ptr, &tmp, 1 );

	return datalen + 5;
}

void print_st( void *p, int len )
{
	int i;
	unsigned char *ptr = (unsigned char*)p;
	for( i = 0; i < len; ++i )
		printf( "%02x\n", *ptr++ );
}

int main( int argc, char **argv )
{
	int fd;
	char buf[256];
	int len;
	char databuf[256];
	int datalen;
	ssize_t count;
	char *ptr;
	pid_t pid;

	if( (fd = open_serial(argv[1], 115200)) < 0 ) {
		perror( "open serial error" );
		exit(-1);
	}

	pid = fork();
	if( 0 == pid ) {
		uint8 data;
		while( read(fd, &data, 1) == 1 )
			printf( "%02x\n", data );
	}

#if 0
	printf( "test SYS_RESET_REQ\n" );
	data = 1;
	len = make_zigbee_frame( 0x41, 0x00, 1, &data, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial\n", count );

	sleep(3);
	pid = fork();
	if( 0 == pid ) {
		while( read(fd, &data, 1) == 1 )
			printf( "%x\n", data );
	}


	sleep(3);
	printf( "test SYS_VERSION\n" );
	len = make_zigbee_frame( 0x21, 0x02, 0, NULL, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial\n", count );


	sleep(3);
	printf( "test SYS_PING\n" );
	len = make_zigbee_frame( 0x21, 0x01, 0, NULL, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial\n", count );


	sleep(3);
	printf( "test SYS_GET_TIME\n" );
	len = make_zigbee_frame( 0x21, 0x11, 0, NULL, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial\n", count );
#endif

#if 0
	printf( "init SYS_OSAL_NV_LENGTH\n" );
	databuf[0] = 0x8F;	
	databuf[1] = 0x0;	// id
	len = make_zigbee_frame( 0x21, 0x013, 2, databuf, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );
#endif

	// enable callbacks
#if 0
	getchar();
	printf( "init SYS_OSAL_NV_ITEM_INIT\n" );
	databuf[0] = 0x8F;	
	databuf[1] = 0x0;	// id
	databuf[2] = 1;
	databuf[3] = 0;		// itemlen
	databuf[4] = 1;		// initlen
	databuf[5] = 1;		// initdata
	len = make_zigbee_frame( 0x21, 0x07, 6, databuf, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );	
#endif

	getchar();
	printf( "read ZCD_NV_ZDO_DIRECT_CB\n" );
	databuf[0] = 0x8F;
	databuf[1] = 0x0;
	databuf[2] = 0;		// offset
	len = make_zigbee_frame( 0x21, 0x08, 3, databuf, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );

#if 0
	getchar();
	printf( "set ZCD_NV_ZDO_DIRECT_CB\n" );
	databuf[0] = 0x8F;
	databuf[1] = 0x00;	//id
	databuf[2] = 0;		// offset
	databuf[3] = 0x01;	// len
	databuf[4] = 0x01;	// value	
	len = make_zigbee_frame( 0x21, 0x09, 5, databuf, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );

	getchar();
	printf( "read ZCD_NV_ZDO_DIRECT_CB\n" );
	databuf[0] = 0x8F;
	databuf[1] = 0x0;
	databuf[2] = 0;		// offset
	len = make_zigbee_frame( 0x21, 0x08, 3, databuf, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );

	// subscribe all callbacks
	getchar();
	printf( "test UTIL_CALLBACK_SUB_CMD\n" );
	ptr = databuf;
	*ptr++ = 0xFF;
	*ptr++ = 0xFF;
	*ptr++ = 1;
	len = make_zigbee_frame( 0x27, 0x06, 3, databuf, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );
#endif

	// SAPI test
	getchar();
	printf( "test ZB_SYSTEM_RESET\n" );
	len = make_zigbee_frame( 0x46, 0x09, 0, NULL, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );


	getchar();
	printf( "test ZB_START_REQUEST\n" );
	len = make_zigbee_frame( 0x26, 0x00, 0, NULL, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );
	// return ZDO_STATE_CHANGE_IND, 0x45, 0xc0, state for 3 times


	getchar();
	printf( "test ZDO_IEEE_ADDR_REQ\n" );
	memset( databuf, 0, sizeof(databuf) );
	len = make_zigbee_frame( 0x25, 0x01, 4, databuf, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );

#if 0
	getchar();
	printf( "test ZB_GET_DEVICE_INFO\n" );
	databuf[0] = 6;
	len = make_zigbee_frame( 0x26, 0x06, 1, databuf, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );
#endif

#if 0
	getchar();
	printf( "test MAC_SCAN_REQ\n" );
	memset( databuf, 0, sizeof(databuf) );
	ptr = databuf;
//	*ptr++ = 0x07;
//	*ptr++ = 0xFF;
//	*ptr++ = 0xF8;
//	*ptr++ = 0x00;
	uint32 scanChannels = 0x07FFF800;
	memcpy( ptr, &scanChannels, sizeof(scanChannels) );
	ptr += sizeof(scanChannels);
	// scan type = 0 energy detect
	*ptr++ = 0;
	// scan duration = 4
	*ptr++ = 4;
	// channel page = 0; ?
	*ptr++ = 0;
	// max result
	*ptr++ = 0xFF;
	// key source
	*ptr++ = 0xF0;
	*ptr++ = 0x61;
	*ptr++ = 0xDC;
	*ptr++ = 0x01;
	*ptr++ = 0x00;
	*ptr++ = 0x4B;
	*ptr++ = 0x12;
	*ptr++ = 0x00;
	// others left 0
	len = make_zigbee_frame( 0x22, 0x0C, 0x13, databuf, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial, waiting for response...\n", count );
#endif

	waitpid( pid, NULL, 0 );
	close_serial(fd);
	return 0;
}

