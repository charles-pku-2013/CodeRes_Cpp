#include <cstdio>
#include <omp.h>

/*
 * k=101
 * k=104
 * k=102
 * k=103
 * last k=100
 */
static
void first_private()
{
    int k = 100;

#pragma omp parallel for firstprivate(k) 
    for ( int i = 1; i <= 4; i++) {
        k += i;
        printf("k=%d\n",k); 
    } // for
    printf("last k=%d\n", k);
}

/*
 * k=101
 * k=103
 * k=104
 * k=102
 * last k=104
 */
static
void first_last_private()
{
    int k = 100;

#pragma omp parallel for firstprivate(k), lastprivate(k)
    for ( int i = 1; i <= 4; i++) {
        k += i;
        printf("k=%d\n",k); 
    } // for
    printf("last k=%d\n", k);
}


int main()
{
    // first_private();
    first_last_private();

    return 0;
}
