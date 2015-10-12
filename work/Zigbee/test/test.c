#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>


typedef unsigned char uint8;

static struct termios oldtio, newtio;

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

	return fd;
}

int close_serial( int fd )
{
	tcsetattr(fd, TCSANOW, &oldtio);
	close(fd);
}

int make_zigbee_frame( uint8 cmd_type, uint8 cmd_sys, uint8 cmd_id, 
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

	// build cmd
	tmp = cmd_type;
	tmp <<= 5;
	tmp |= cmd_sys;

	// write cmd
	memcpy( ptr++, &tmp, 1 );
	memcpy( ptr++, &cmd_id, 1 );

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
		printf( "%x\n", *ptr++ );
}

int main( int argc, char **argv )
{
	int fd;
	char buf[256];
	int len;
	uint8 data;
	ssize_t count;
	sigset_t sigset;

	if( (fd = open_serial(argv[1], 115200)) < 0 ) {
		perror( "open serial error" );
		exit(-1);
	}

	len = make_zigbee_frame( 1, 1, 2, 0, NULL, buf );
	print_st( buf, len );
	count = write(fd, buf, len);
	if( count < 0 ) {
		perror( "write serial error!" );
		exit(-1);
	}
	printf( "%d byets written to serial\n", count );

	printf( "waitting for response...\n" );
	while( read(fd, &data, 1) == 1 )
		printf( "%x\n", data );

	close_serial(fd);
	return 0;
}

