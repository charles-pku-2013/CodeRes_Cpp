#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char ch;
	int i;

//	setbuf( stdin, NULL );

	for( i = 0; i < 20; ++i ) {
		ch = getchar();
		putchar( ch );
	}
	
	return 0;
}

