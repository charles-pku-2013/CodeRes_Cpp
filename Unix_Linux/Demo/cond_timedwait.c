#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	cond = PTHREAD_COND_INITIALIZER;
volatile uint8	waitSignal = 0;

void *routine( void *arg )
{
	printf( "thread %u started!\n", (uint32)pthread_self() );
	sleep(5);
	printf( "thread %u will raise the signal.\n", (uint32)pthread_self() );
	pthread_mutex_lock( &lock );
	waitSignal = 1;
	pthread_mutex_unlock( &lock );
	pthread_cond_signal( &cond );
	
	return 0;
}

int main()
{
	pthread_t		tid;
	int				err;
	struct timeval	tv;	
	struct timespec	ts;
	
	gettimeofday( &tv, NULL );
	ts.tv_sec  = tv.tv_sec + 3;		/* 3 seconds in future */
	ts.tv_nsec = tv.tv_usec * 1000;
	
	printf( "main thread: %u started!\n", (uint32)pthread_self() );
	pthread_create( &tid, NULL, routine, NULL );

	printf( "main thread: %u waiting for signal...\n", (uint32)pthread_self() );
	pthread_mutex_lock( &lock );
	while( !waitSignal ) {
		err = pthread_cond_timedwait( &cond, &lock, &ts );
		if( ETIMEDOUT == err ) {
			printf( "wait cond time out!\n" );
			goto END;
		} // if
	}
	printf( "main thread: %u got the signal.\n", (uint32)pthread_self() );
	pthread_mutex_unlock( &lock );
	
END:
	pthread_mutex_unlock( &lock );
	pthread_join( tid, NULL );
	printf( "main thread: %u terminates!\n", (uint32)pthread_self() );
	return 0;
}


















