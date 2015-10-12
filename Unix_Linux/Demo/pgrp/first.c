#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char **argv )
{
	printf( "This is process %s %ld belongs to group %ld\n", argv[0], (long)getpid(), (long)getpgrp() );
	return 0;
}
