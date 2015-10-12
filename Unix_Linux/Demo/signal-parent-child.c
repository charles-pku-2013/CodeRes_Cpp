#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sig_handler( int signo )
{
	printf( "process %d received the signal %d\n", getpid(), signo );
}

int main()
{
	pid_t pid;
	
	printf( "main process %d starts!\n", getpid() );
	
	signal( SIGINT, sig_handler );
	
	if( (pid = fork() ) < 0 )
		perror( "fork error" );
	else if( 0 == pid ) {
		printf( "child process %d starts!\n", getpid() );
		pause();
		printf( "child process %d ends!\n", getpid() );
		_exit(0);
	}
	
	pause();
	
	printf( "main process %d ends!\n", getpid() );
	return 0;
}





