#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


#define TIMEOUT 			10
#define N_PROCESS 			9

#ifdef _DEBUG
#define DBG( fmt, ... ) fprintf( stdout, fmt, ##__VA_ARGS__ )
#else
#define DBG
#endif

static char 			cmdStr[N_PROCESS][80];
static pid_t			processes[N_PROCESS];
static int 				n_fail = 0;


inline static
void my_exit( int retval )
{
	system( "killall -q ping nslookup" );
	exit(retval);
}

inline static
void init( const char *gw )
{
	memset( processes, 0, sizeof(processes) );

	sprintf( cmdStr[0], "ping -c1 %s", gw );
//	sprintf( cmdStr[0], "ping %s", "www.google.com" );
	sprintf( cmdStr[1], "nslookup root1.gaaiho.com" );
	sprintf( cmdStr[2], "nslookup root2.gaaiho.com 8.8.8.8" );
	sprintf( cmdStr[3], "nslookup root3.gaaiho.com" );
	sprintf( cmdStr[4], "nslookup root4.gaaiho.com 8.8.8.8" );
	sprintf( cmdStr[5], "nslookup root5.gaaiho.com" );
	sprintf( cmdStr[6], "nslookup root6.gaaiho.com 8.8.4.4" );
	sprintf( cmdStr[7], "nslookup root7.gaaiho.com" );
	sprintf( cmdStr[8], "nslookup root8.gaaiho.com 8.8.4.4" );
}


inline static
void kill_all()
{
	int i;
	for( i = 0; i < N_PROCESS; ++i )
		if( processes[i] )
			kill( processes[i], SIGKILL );
}


inline static
void onChildFinish()
{
	int i, stat;
	pid_t pid;

	while( (pid = waitpid(-1, &stat, WNOHANG)) > 0 ) {
		DBG( "process %ld finishes with retcode %d.\n", pid, WEXITSTATUS(stat) );
		for( i = 0; i < N_PROCESS; ++i ) {
			if( pid == processes[i] ) {
				processes[i] = 0;
				break;
			} // if	
		} // for

		if( 0 == WEXITSTATUS(stat) ) {
			kill_all();
			my_exit(0);
		} else {
			if( ++n_fail == N_PROCESS ) {
				DBG( "all fail, terminate!\n" );
				my_exit(-1);
			}
		} // if
	}  // while
}


inline static
void onTimeOut()
{
	DBG( "Wait Timeout, terminate!\n" );
	kill_all();
	my_exit(-1);
}


int main( int argc, char **argv )
{
	sigset_t 		newset, oldset;
	int 			i, signo;
	pid_t 			pid;

	if( argc < 2 ) {
		printf( "usage: check_connection gatewayIp\n" );
		exit(-1);
	}
	
	sigemptyset( &newset );
	sigaddset( &newset, SIGCHLD );
	sigaddset( &newset, SIGALRM );
//	sigfillset( &newset );
	pthread_sigmask( SIG_BLOCK, &newset, &oldset );

	init( argv[1] );

	alarm( TIMEOUT );

	for( i = 0; i < N_PROCESS; ++i ) {
		pid = fork();
		if( pid < 0 ) {
			continue;
		} else if( 0 == pid ) {		// child
			DBG( "Child process %ld running cmd %s\n", getpid(), cmdStr[i] );
			execl( "/bin/sh", "sh", "-c", cmdStr[i], (char*)NULL );
			_exit(127);
//			sleep(1);
//			_exit(-1);
		} else {			// parent
			processes[i] = pid;
		} // if pid
	} // for
	

	while( sigwait(&newset, &signo) == 0 ) {
		switch( signo ) {
		case SIGCHLD:
			onChildFinish();
			break;
		case SIGALRM:
			onTimeOut();
			break;
//		default:
//			printf( "received unknown signal %d\n", signo );
		} // switch
	} // while
	
	return 0;
}

