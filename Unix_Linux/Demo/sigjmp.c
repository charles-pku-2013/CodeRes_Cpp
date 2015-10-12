// apue p318 fg10.11

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/types.h>

static sigjmp_buf	jmpbuf;
static volatile sig_atomic_t canjmp;

void sig_handler( int signo )
{
	printf( "signal %i is caught!\n", signo );
	canjmp = 0;
	siglongjmp( jmpbuf, 1 );
}

int main()
{
	int retval;

	printf( "test process %i started!\n", getpid() );

	if( signal( SIGUSR1, sig_handler ) == SIG_ERR ) {
		printf( "install signal error\n" );
		exit(-1);
	}

//!! paragon of using alarm
	retval = sigsetjmp(jmpbuf, 1);
	canjmp = 1;		//!! prevent signal arriving between signal() sigsetjmp()
	printf( "sigsetjmp returned %i\n", retval );
	if( retval )
		goto END;

	printf( "now main process goes to sleep!\n" );
	pause();
END:
	printf( "main terminates!\n" );
	return 0;
}
