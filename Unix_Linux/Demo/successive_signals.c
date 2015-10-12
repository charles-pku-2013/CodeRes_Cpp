#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

static
void sig_handler( int signo )
{
	printf( "signal processing starts!\n" );
	sleep(5);		// press Ctrl+C here many times
	printf( "signal processing ends!\n" );
}

int main()
{
	signal( SIGINT, sig_handler );
	
	for( ; ; )
		pause();
	
	return 0;
}
