#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

double compute( const char *sx, const char *sy, const char *sop )
{
    int x = atoi(sx);
    int y = atoi(sy);
    double z;
    
    switch( sop[0] ) {
        case '+':
            z = x + y;
            break;
        case '-':
            z = x - y;
            break;
        case '*':
            z = x * y;
            break;
        case '/':
            z = (double)x / y;
            break;
    } // switch
    
    return fabs( z - 9.0 );
}

int main()
{
    int n, i;
    char sx[20], sy[20], sop[20];
    int n_case = 0;
    double mindiff = 65535, diff;
    
    scanf( "%d\n", &n );
    for( i = 1; i <= n; ++i ) {
        scanf( "%s %s %s\n", sx, sop, sy );
        diff = compute( sx, sy, sop );
        if( diff < mindiff ) {
            mindiff = diff;
            n_case = i;
        } // if
    } // for
    
    printf("%d\n", n_case);
    
    return 0;
}














