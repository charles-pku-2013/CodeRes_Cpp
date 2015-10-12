#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void normalize_tm( struct tm *tm )
{
    time_t tv = mktime( tm );
    localtime_r( &tv, tm );
}


int main(void)
{
    struct tm tm;
    char buf[255];

    memset(&tm, 0, sizeof(struct tm));
    strptime("00:59:59", "%H:%M:%S", &tm);
    tm.tm_sec += 10;
    normalize_tm( &tm );
    strftime(buf, sizeof(buf), "%H:%M:%S", &tm);
    puts(buf);

    exit(EXIT_SUCCESS);
}

