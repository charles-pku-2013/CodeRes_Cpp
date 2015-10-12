#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


#ifdef _DEBUG
#define DBG( fmt, ... ) fprintf( stdout, "wifi_test: "fmt, ##__VA_ARGS__ )
#else
#define DBG
#endif


enum { STATE_NULL, STATE_AUTH_COMPLETE, STATE_AUTH_FAIL, STATE_CONNECTED, STATE_DISCONNECTED, STATE_TIMEOUT };
static const char *infoStr[] = {
							"",
							"Key negotiation completed",
							"pre-shared key may be incorrect",
							"CTRL-EVENT-CONNECTED",
							"CTRL-EVENT-DISCONNECTED",
							"Authentication with"		// and "timed out" int the end
						};

static int 	state = STATE_NULL;

static 
void sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	exit(1);
}


//inline static
//void sendCmd( const char *cmd )
//{
//	sprintf( cmdStr, "%s\n", cmd );
//	write( fd1[1], cmdStr, strlen(cmdStr) );
//}

inline static
void my_exit( int retval )
{
	system( "killall wpa_cli" );
	exit( retval );
}


static
void parseOutput( FILE *fp )
{
	char line[1024];
	char *p;

	while( fgets(line, 1024, fp) ) {
		DBG( "%s", line );
		if( p = strstr(line, infoStr[STATE_AUTH_COMPLETE]) ) {
			state = STATE_AUTH_COMPLETE;
		} else if( p = strstr(line, infoStr[STATE_AUTH_FAIL]) ) {
			state = STATE_AUTH_FAIL;
		} else if( p = strstr(line, infoStr[STATE_CONNECTED]) ) {
			if( STATE_AUTH_COMPLETE == state )
				my_exit(0);					// success
			state = STATE_CONNECTED;
		} else if( p = strstr(line, infoStr[STATE_DISCONNECTED]) ) {
			if( STATE_AUTH_FAIL == state )
				my_exit(2);
			state = STATE_DISCONNECTED;
		} else if( p = strstr(line, "timed out") ) {
			DBG( "cannot connect to wifi router.\n" );
			my_exit(1);
		}
	} // while

	return;
}

int main( int argc, char **argv )
{
	char ch;

//	FILE *fp;
//	fp = fopen( "/tmp/wifilog", "w" );
//	fp = stdout;
//	setbuf( fp ,NULL );

//	if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
//		exit(-1);
	
	setbuf( stdin, NULL );

//	while( (ch = fgetc(stdin)) != (char)EOF )
//		fputc(ch, fp);

	parseOutput( stdin );

	return 0;
}

