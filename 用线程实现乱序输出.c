#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

char *str = "abcdefg";


void* thr_func( void *arg )
{
	int i = (int)arg;

	sleep(1);
	printf("%c", str[i]);
	
	return (void*)0;
}


int main()
{
	int i;
	pthread_t *tidArray = 0;
	
	tidArray = (pthread_t*)malloc( strlen(str)*sizeof(pthread_t) );
	
	for( i = 0; i < strlen(str); ++i )
		pthread_create( tidArray+i, NULL, thr_func, (void*)i );
		
	for( i = 0; i < strlen(str); ++i )
		pthread_join( tidArray[i], NULL );
	
	free(tidArray);
    
    putchar('\n');
    
    return 0;
}














