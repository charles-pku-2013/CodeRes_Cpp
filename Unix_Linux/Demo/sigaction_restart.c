#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define MAXLINE 1024

void sig_handler( int signo )
{
	printf( "signal %d caught!\n", signo );
}

int main()
{
	int n;
	char buf[MAXLINE];
	struct sigaction act, oact;

	memset( &act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	act.sa_flags |= SA_RESTART;	//!! The default action in Linux and Mac OS is to terminate the read sys call
	sigaction( SIGALRM, &act, &oact );

	alarm(5);
	
	while( (n = read(STDIN_FILENO, buf, MAXLINE)) > 0 )
		write( STDOUT_FILENO, buf, n );

	printf( "main process terminates!\n" );
	return 0;
}
