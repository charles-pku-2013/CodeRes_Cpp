#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


int main()
{
    struct timeval start, finish, elapsed;
    double duration;
    unsigned int i = 0;
    
    gettimeofday( &start, NULL );
    while( ++i );
    gettimeofday( &finish, NULL );
    
    timersub( &finish, &start, &elapsed );
    duration = elapsed.tv_sec + (double)(elapsed.tv_usec) / 1000000;
    printf("duration = %lf seconds\n", duration);
    
    return 0;
}














