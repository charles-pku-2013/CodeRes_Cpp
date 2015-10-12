#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
	pid_t pid;
	int status;

	printf( "parent program %d starts!\n", getpid() );
	
	sigset_t set, oset;
	sigemptyset( &set );
	sigaddset( &set, SIGUSR1 );
	sigprocmask( SIG_BLOCK, &set, &oset );
	
	if( (pid = fork()) < 0 )
		perror( "fork fail" );
	else if( 0 == pid ) {
		sleep(10);
		printf( "Now child process %d launches a new program through exec.\n", getpid() );
		execl( "/bin/sh", "sh", "-c", "./exec-prog", (char*)0 );
		_exit(0);
	}
	
	waitpid( -1, &status, 0 );
	
	printf( "parent program %d ends!\n", getpid() );
	
	return 0;
}
