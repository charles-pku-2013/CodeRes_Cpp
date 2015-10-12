#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFLEN 1024

static char buf[1024];

int main( int argc, char **argv )
{
	int n;
	
	if( (n = read(STDIN_FILENO, buf, BUFLEN)) < 0 )
		perror( "read error" );
	if( n >= BUFLEN )
		n = BUFLEN - 1;
	buf[n] = 0;
	
	printf( "process %s %ld belongs to group %ld receives msg: %s\n", argv[0], (long)getpid(), (long)getpgrp(), buf );
	
	return 0;
}
