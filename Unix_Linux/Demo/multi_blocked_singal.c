#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

static
void sig_usr1( int signo )
{
	printf( "SIGUSR1 handler begins!\n" );
	sleep(5);
	printf( "SIGUSR1 handler ends!\n" );
}

static
void sig_usr2( int signo )
{
	printf( "SIGUSR2 handler begins!\n" );
	sleep(5);
	printf( "SIGUSR2 handler ends!\n" );
}

static
void sig_int( int signo )
{
	printf( "SIGINT handler begins!\n" );
	sleep(5);
	printf( "SIGINT handler ends!\n" );
}

int main()
{
	sigset_t newset, oldset;
	int signo;

	setvbuf(stdout, NULL, _IONBF, 0);
	
	sigemptyset( &newset );
	sigaddset( &newset, SIGUSR1 );
	sigaddset( &newset, SIGUSR2 );
	sigaddset( &newset, SIGINT );

	sigprocmask( SIG_BLOCK, &newset, &oldset );

	signal( SIGUSR1, sig_usr1 );
	signal( SIGUSR2, sig_usr2 );
	signal( SIGINT, sig_int );

	printf( "main thread is going to sleep!\n" );
	sleep(10);
	printf( "main thread is going to restore the previous mask!\n" );

	sigprocmask( SIG_SETMASK, &oldset, NULL );
	pause();

	printf( "main thread finishes!\n" );
	
	return 0;
}





