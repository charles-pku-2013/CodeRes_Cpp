#include <stdio.h>
#include <string.h>

char buf[256];

int main()
{
	printf( "child process for testing start!\n" );
	while( fgets(buf, 256, stdin) )
		printf( "child process got the following message:\n%s", buf );
	return 0;
}
