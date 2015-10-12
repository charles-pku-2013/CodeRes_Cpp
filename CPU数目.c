#include <stdio.h>
#include <unistd.h>

#define N_PROCESSORS sysconf( _SC_NPROCESSORS_ONLN )


int main()
{
    printf( "%ld\n", N_PROCESSORS );
    
    return 0;
}














