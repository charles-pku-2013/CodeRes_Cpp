#include <cstdio>
#include <omp.h>


int main()
{
    int sum = 1000;

#pragma omp parallel for reduction(+: sum) 
    for ( int i = 1; i <= 100; i++ ) {
        sum += i; 
    } // for
    printf( "sum = %d\n", sum);

    return 0;
}

// sum = 6050
