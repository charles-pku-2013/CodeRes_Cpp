#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>



int main( int argc, char **argv )
{
    setlocale(LC_ALL, "zh_CN.UTF-8");       //!! 必须有
    
    wchar_t s[] = L"中国";
    wprintf(L"%ls\n", s);
    
    printf( "%lu\n", wcslen(s) );
    
    for( wchar_t *p = s; *p; ++p )
        wprintf(  L"%lc\n", *p );
    putchar('\n');
    
    return 0;
}












