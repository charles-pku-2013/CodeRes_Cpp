#include <unistd.h>
#include <stdio.h>

int main()
{
    unsigned int i = 0;

    setvbuf( stdout, NULL, _IONBF, 0 );

    while( 1 ) {
        ++i;
        fwrite( &i, sizeof(i), 1, stdout );
        fprintf(stderr, "write %d\n", i);
        /* sleep(1); */
        usleep( 100000 );
    } // while

    return 0;
}
