/* cat_noblock.c - open a file and display its contents, but exit rather than
 * wait for input */


/* Copyright (C) 1998 by Ori Pomerantz */



#include <stdio.h>    /* standard I/O */
#include <fcntl.h>    /* for open */
#include <unistd.h>   /* for read */ 
#include <stdlib.h>   /* for exit */
#include <errno.h>    /* for errno */
#include <signal.h>
#include <sys/types.h>

void sig_handle( int signo )
{
	printf( "caught signal %d\n", signo );
}

main(int argc, char *argv[])
{
	int    fd;  /* The file descriptor for the file to read */
	
	signal( SIGINT, sig_handle );

	/* Open the file for reading in non blocking mode */ 
//	fd = open("/dev/testdev", O_RDONLY | O_NONBLOCK);
	fd = open("/dev/testdev", O_RDONLY);
	
	if( fd < 0 ) {
		perror( "open device fail: " );
		exit(-1);
	}
	
	printf( "I've got the device!\n" );
//	close( fd );

	return 0;
}




