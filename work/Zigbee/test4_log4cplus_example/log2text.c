#include <stdio.h>
#include <ctype.h>

int main()
{
	char ch;
	int state = 0; // 0 for print word 1 for print data
	
	while( scanf( "%x", &ch ) == 1 ) {
		if( isprint(ch) )
			putchar(ch);
		else
			printf( "%02x ", (unsigned char)ch );
	} // while
	
	putchar( '\n' );
	
	return 0;
}



