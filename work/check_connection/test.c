#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
	pid_t pid;
	
	pid = fork();
	
	signal( SIGCHLD, sig_chld );
	
	if( 0 == pid ) {
		printf( "child process %ld running.\n", getpid() );
		sleep(1);
		printf( "child process %ld terminates.\n", getpid() );
		_exit(0);
	}
	
	printf( "parent process %ld waiting...\n", getpid() );
	pause();
	
	return 0;
}







