#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define TIMEOUT 20

#ifdef _DEBUG
#define DBG( fmt, ... ) fprintf( stdout, "wifi_test: "fmt, ##__VA_ARGS__ )
#else
#define DBG
#endif


enum { STATE_INIT, STATE_ASSOCIATING, STATE_ASSOCIATED, STATE_CONNECTED,
		STATE_DISCONNECTED, STATE_WRONG_PASSWORD, STATE_TIMEOUT };
static const char *infoStr[] = {
							"",
							"Trying to associate with",
							"Associated with",
							"CTRL-EVENT-CONNECTED",
							"CTRL-EVENT-DISCONNECTED",
							"pre-shared key may be incorrect",
							"Authentication with"			// and "timed out"
						};

static char cmdStr[256];
static int 	fd1[2], fd2[2];
static int 	state = STATE_INIT;

static 
void sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	exit(1);
}


inline static
void sendCmd( const char *cmd )
{
	sprintf( cmdStr, "%s\n", cmd );
	write( fd1[1], cmdStr, strlen(cmdStr) );
}

inline static
void my_exit( int retval )
{
	sendCmd( "q" );
#ifdef _DEBUG
	if( 0 == retval )
		DBG( "CONNECT SUCCESS\n" );
	else if( 1 == retval )
		DBG( "CONNECT FAIL\n" );
	else if( 2 == retval )
		DBG( "WRONG PASSWORD\n" );
#endif
	exit( retval );
}

static
void sig_alrm( int signo )
{
	DBG( "WAIT TIMEOUT\n" );
	my_exit(1);
}


static
void parseOutput( FILE *fp )
{
	char line[1024];
	char *p;

	while( fgets(line, 1024, fp) ) {
		DBG( "%s", line );
//		DBG( "line size = %d\n", strlen(line) );
		if( p = strstr(line, infoStr[STATE_ASSOCIATING]) ) {
			DBG( "Set state to STATE_ASSOCIATING\n" );
			state = STATE_ASSOCIATING;
		} else if( p = strstr(line, infoStr[STATE_ASSOCIATED]) ) {
			DBG( "Set state to STATE_ASSOCIATED\n" );
			state = STATE_ASSOCIATED;
		} else if( p = strstr(line, infoStr[STATE_CONNECTED]) ) {
			DBG( "Deal with STATE_CONNECTED\n" );
//			if( STATE_ASSOCIATED == state )
			my_exit(0);
			state = STATE_CONNECTED;
		} else if( p = strstr(line, infoStr[STATE_WRONG_PASSWORD]) ) {
			state = STATE_WRONG_PASSWORD;
			my_exit(2);
		} else if( p = strstr(line, infoStr[STATE_DISCONNECTED]) ) {
//			if( STATE_WRONG_PASSWORD == state )
//				my_exit(2);
//			else if( STATE_INIT != state )
//				my_exit(1);
			state = STATE_DISCONNECTED;
		} else if( p = strstr(line, infoStr[STATE_TIMEOUT]) ) {
			p += strlen(infoStr[STATE_TIMEOUT]);
			if( p = strstr(p, "timed out") ) {
				if( STATE_ASSOCIATING == state )
					my_exit(1);
				state = STATE_TIMEOUT;
			} // p
		} // if
	} // while

	return;
}

int main( int argc, char **argv )
{
	pid_t	pid;
	FILE	*fp;

	if( argc < 4 ) {
		printf( "wrong usage\n" );
		exit(2);
	}

	if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
		exit(-1);
	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		exit(-1);

	alarm( TIMEOUT );	

	sprintf( cmdStr, "wpa_supplicant %s %s %s -B", argv[1], argv[2], argv[3] );
	if( system(cmdStr) ) {
		printf( "cannot start wpa_supplicant!\n" );
		exit(-1);
	}

	if (pipe(fd1) < 0 || pipe(fd2) < 0)
		exit(-1);

	if ((pid = fork()) < 0) {
		exit(-1);
	} else if( 0 == pid ) {									/* child */
		DBG( "Child process started.\n" );
		
		close(fd1[1]);
		close(fd2[0]);
		
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
				exit(-1);
			close(fd1[0]);
		}

		if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
				exit(-1);
			close(fd2[1]);
		}

		sprintf( cmdStr, "pty wpa_cli %s", argv[1] );
		if (execl("/bin/sh", "sh", "-c", cmdStr, (char *)0) < 0)
			exit(-1);
	} // if

	// main process
	close(fd1[0]);
	close(fd2[1]);

	if( (fp = fdopen(fd2[0], "r")) == NULL ) {
		printf( "cannot open file descriptor of read pipe of child process." );
		exit(-1);
	}
	setlinebuf( fp );

	parseOutput( fp );

	return 0;
}



