#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sig_int( int signo )
{
	printf( "signal SIGINT caught!\n" );
}

void sig_usr1( int signo )
{
	printf( "signal SIGUSR1 caught!\n" );
}

int main()
{
	sigset_t newset, oldset, pendingset;
	
	printf( "main starts %d!\n", getpid() );
	
	sigemptyset( &newset );
	sigaddset( &newset, SIGINT );
	sigaddset( &newset, SIGUSR1 );
	
	sigprocmask( SIG_BLOCK, &newset, &oldset );
	
	signal( SIGINT, sig_int );
	signal( SIGUSR1, sig_usr1 );
	
	printf( "main goes to sleep!\n" );
	sleep(5);
	
	sigpending( &pendingset );

	if( sigismember( &pendingset, SIGINT ) )
		printf( "SIGINT pending\n" );

	sigprocmask( SIG_SETMASK, &oldset, NULL );
	
	printf( "main ends!\n" );
	
	return 0;
}







