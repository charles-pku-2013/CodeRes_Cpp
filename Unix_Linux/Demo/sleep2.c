#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <setjmp.h>

static jmp_buf jmpbuf;

static
void sig_alarm( int signo )
{
	longjmp(jmpbuf, 1);
}

void sig_int( int signo )
{
	printf( "SIGINT handler starts!\n" );
	sleep(10);
	printf( "SIGINT handler ends!\n" );
}

unsigned int sleep2( unsigned int nsecs )
{
	if( signal(SIGALRM, sig_alarm) == SIG_ERR )
		return nsecs;
	if( setjmp(jmpbuf) == 0 ) {
		alarm( nsecs );
		pause();
	}
	return alarm(0);
}

int main()
{
	signal( SIGINT, sig_int );
	printf( "main process %d starts!\n", getpid() );
	sleep2(5);
	printf( "main process %d ends!\n", getpid() );
	
	return 0;
}





















