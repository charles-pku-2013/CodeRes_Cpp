#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

/*
 * !! When compile, -lpthread must be provided, otherwise the mutex lock will not work
*/


/*
 * A thread-safe but non-reentrant function.
*/
void demo_func( const char *msg )
{
	static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	
	printf( "In demo_func: %s\n", msg );
	pthread_mutex_lock( &lock );
	printf( "demo_func in lock area %s\n", msg );
	sleep(5);
	pthread_mutex_unlock( &lock );
	printf( "demo_func end %s\n", msg );
}


void sig_int( int signo )
{
	printf( "In SIGINT handler\n" );
	demo_func( "from sig_int" );
}


int main( int argc, char **argv )
{
	signal( SIGINT, sig_int );
	
	printf( "main starts...\n" );
	printf( "main Call demo_func()\n" );
	
	demo_func( "from main()" );
	
	return 0;
}



/* 
main starts...
main Call demo_func()
In demo_func: from main()
demo_func in lock area from main()
^CIn SIGINT handler
In demo_func: from sig_int
 */















