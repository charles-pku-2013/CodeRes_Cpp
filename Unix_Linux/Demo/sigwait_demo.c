#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
	sigset_t newset, oldset;
	int signo;
	
	sigemptyset( &newset );
	sigaddset( &newset, SIGUSR1 );
	sigaddset( &newset, SIGUSR2 );

	sigprocmask( SIG_BLOCK, &newset, &oldset );

	while( sigwait(&newset, &signo) == 0 ) {
		switch( signo ) {
		case SIGUSR1:
			printf( "received signal SIGUSR1!\n" );
			sleep(10);
			printf( "processing of signal SIGUSR1 ends!\n" );
			break;
		case SIGUSR2:
			printf( "received signal SIGUSR2!\n" );
			break;
		} // switch
	} // while
	
	return 0;
}





