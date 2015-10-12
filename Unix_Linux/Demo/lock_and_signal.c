#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


static
void sig_int( int signo )
{
	printf( "SIGINT received!\n" );
}


static
void *thread_func( void *arg )
{
	printf( "thread_func start...\n" );
	pthread_mutex_lock( &lock );
	sleep( 10 );
	pthread_mutex_unlock( &lock );
	printf( "thread_func end...\n" );
	return NULL;
}


int
main(void)
{
	pthread_t tid;

	signal( SIGINT, sig_int );
	
	pthread_create( &tid, NULL, thread_func, NULL );
	sleep(1);
	
	printf( "main thread is trying to acquire the lock.\n" );
	
	pthread_mutex_lock( &lock );
	printf( "main thread got the lock.\n" );
	pthread_mutex_unlock( &lock ); 
	
	printf( "main program terminates...\n" );
	return 0;
}

























