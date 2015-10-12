#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

static
void sig_pipe( int signo )
{
	fprintf( stderr, "SIGPIPE received, terminate!\n" );
	exit(-1);
}

int main()
{
	int count = 0;

	setbuf( stdout, NULL );			//!! must do this, otherwise process at the other end of the pipe cannot read data immediately!
	
	signal( SIGPIPE, sig_pipe );

	for( ; ; ) {
		printf( "%d msg produced.\n", ++count );
		sleep(1);
	}
	
	return 0;
}
