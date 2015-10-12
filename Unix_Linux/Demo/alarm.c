// apue p318 fg10.11

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/types.h>

static sigjmp_buf	jmpbuf;

//!! Other signal has no influence on SIGALRM

void sig_handler( int signo )
{
	if( SIGALRM == signo ) {
		printf( "SIGALRM caught!\n" );
		siglongjmp( jmpbuf, 1 );
	} else {
		printf( "signal %i is caught!\n", signo );
	}
}

int main()
{
	printf( "test process %i started!\n", getpid() );

	signal( SIGALRM, sig_handler );
	signal( SIGUSR1, sig_handler );

//!! paragon of using alarm
	if( sigsetjmp(jmpbuf, 1) )
		goto END;

	alarm( 10 );

	for( ; ; )
		pause();
END:
	printf( "main terminates!\n" );
	return 0;
}
