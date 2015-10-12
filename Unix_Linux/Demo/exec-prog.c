#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sig_handler( int signo )
{
	printf( "recevied signal %d\n", signo );
}

int main()
{
	sigset_t curset, waitset;
	
	printf( "exec object program %d starts!\n", getpid() );
	
	sigprocmask( SIG_SETMASK, NULL, &curset );
	
	if( sigismember( &curset, SIGUSR1 ) )
		printf( "current signal mask including SIGUSR1\n" );
	
	sigemptyset( &waitset );
	sigsuspend( &waitset );
	
	printf( "exec object program %d ends!\n", getpid() );
	return 0;
}
