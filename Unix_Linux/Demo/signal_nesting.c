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
	int signo;

	setvbuf(stdout, NULL, _IONBF, 0);

	signal( SIGUSR1, sig_usr1 );
	signal( SIGUSR2, sig_usr2 );
	signal( SIGINT, sig_int );

	printf( "main thread is going to sleep!\n" );

	pause();

	printf( "main thread finishes!\n" );
	
	return 0;
}





