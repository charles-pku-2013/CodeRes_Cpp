#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <setjmp.h>
#include <fcntl.h>


#ifdef _DEBUG
#define DBG( format, ... )  do { \
	fprintf( stdout, format"\n", ##__VA_ARGS__ );	\
} while(0)
#else
#define DBG
#endif

#define SERVER_LIST_FILE "/opt/NTPServers"
#define N_MAX_SERVERS 20
#define LINE_MAX 40
#define TIMEOUT 60 // in secs

static char server_list[N_MAX_SERVERS][LINE_MAX];
static int n_servers;
static int n_fails = 0;
static pid_t processes[N_MAX_SERVERS];

static jmp_buf	jmpbuf;

static
void read_server_list()
{
	FILE *fp;
	char line[LINE_MAX];
	
	if( (fp = fopen(SERVER_LIST_FILE, "r")) == NULL ) {
		DBG( "Cannot open server list file!\n" );
		exit( -1 );
	} // if
	
	n_servers = 0;	
	while( fgets(line, LINE_MAX, fp) ) {
		// no empty line allowed
		if( isspace(line[0]) )
			break;
		if( isspace(line[ strlen(line)-1 ]) )
			line[ strlen(line)-1 ] = 0;
		
		strncpy( server_list[n_servers], line, LINE_MAX );
		
		if( ++n_servers >= N_MAX_SERVERS )
			break;
	}
	
	fclose( fp );
	return;
}

static
void sig_chld( int signo )
{
	pid_t pid;
	int stat, i;
	sigset_t newset, oldset;

	sigemptyset( &newset );
	sigaddset( &newset, SIGALRM );
	sigprocmask( SIG_BLOCK, &newset, &oldset );
	
	while( (pid = waitpid(-1, &stat, WNOHANG) ) > 0 ) {
		for( i = 0; i < n_servers; ++i ) {
			if( processes[i] == pid ) {
				processes[i] = 0;
				break;
			} // if
		} // for
		if( 0 == WEXITSTATUS(stat) ) {	
			DBG( "process %i succeed!", pid );
			longjmp( jmpbuf, 1 );
		} else if( ++n_fails == n_servers ) {
			longjmp( jmpbuf, 2 );
		} // if
	} // while

	sigprocmask( SIG_SETMASK, &oldset, NULL );
	return;	
}

static
void sig_alrm( int signo )
{
	DBG( "wait timeout!" );
	longjmp( jmpbuf, 3 );
}

int main()
{
	int i;
	pid_t pid;

/*
	if( signal(SIGCHLD, sig_chld) == SIG_ERR ) {
		DBG( "install signal handler error" );
		exit(-1);
	}

	if( signal(SIGALRM, sig_alrm) == SIG_ERR ) {
		DBG( "install signal handler error" );
		exit(-1);
	}	
*/

	struct sigaction act, oact;

	act.sa_flags = 0;
	sigemptyset( &act.sa_mask );
	sigaddset( &act.sa_mask, SIGALRM );
	act.sa_handler = sig_chld;
	if( sigaction( SIGCHLD, &act, &oact ) < 0 ) {
		printf( "install signal handler error!\n" );
		exit(-1);
	}

	act.sa_flags = 0;
	sigemptyset( &act.sa_mask );
	sigaddset( &act.sa_mask, SIGCHLD );
	act.sa_handler = sig_alrm;
	if( sigaction( SIGALRM, &act, &oact ) < 0 ) {
		printf( "install signal handler error!\n" );
		exit(-1);
	}

	read_server_list();

	if( !n_servers )
		exit(0);

/*	
	DBG( "n_servers = %d", n_servers );
	for( i = 0; i < n_servers; ++i )
		DBG( "%d, %s", i, server_list[i] );
*/
	

	memset( processes, 0, N_MAX_SERVERS*sizeof(pid_t) );

	if( setjmp(jmpbuf) )
		goto END;

	alarm( TIMEOUT );

	for( i = 0; i < n_servers; ++i ) {
		pid = fork();
		if( pid < 0 )
			perror( "fork the process error!" );
		else if( 0 == pid ) {
			// child
//			sleep(1);
//			DBG( "I am the %dth process %i, %i", i, getpid(), processes[i] );
			char cmd[80];
			sprintf( cmd, "ntpclient -s -h %s", server_list[i] );
//			DBG( "cmd = %s", cmd );
			execl( "/bin/sh", "sh", "-c", cmd, (char*)NULL );
			_exit(127);
		} 
#ifdef _DEBUG
		else {
			processes[i] = pid;
			//DBG( "forked the %dth process %i", i, processes[i] );
		}
#endif		
	} // for

	for( ; ; )
		pause();
END:
	// kill all other processes
	for( i = 0; i < n_servers; ++i ) {
		if( processes[i] )
			kill( processes[i], SIGKILL );
	} // for	
	system( "killall -q ntpclient" );
	DBG( "ntpsync main process terminates!" );
	return 0;
}











