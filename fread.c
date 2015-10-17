#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 13

int main( int argc, char **argv )
{
    FILE *fp = NULL;

    if( (fp = fopen(argv[1], "rb")) == NULL ) {
        fprintf( stderr, "Cannot open file!\n" );
        exit(-1);
    } // if 

    char buf[BUFSIZE];
    size_t nread;

    size_t i;
    while( (nread = fread(buf, 1, BUFSIZE, fp)) > 0 ) {
    /* while( (nread = fread(buf, BUFSIZE, 1, fp)) > 0 ) { */
        printf("nread = %zu\n", nread);
        for( i = 0; i < nread; ++i )
            putchar( buf[i] );
        putchar('\n');
    } // for

    printf("nread = %zu\n", nread);
    putchar('\n');

    if( feof(fp) ) printf( "EOF reached!\n" );
    if( ferror(fp) ) printf( "error occured!\n" );

    return 0;
}
