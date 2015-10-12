#include <stdio.h>
#include <unistd.h>

int main()
{
	int i;
	
//	setbuf( stdout, NULL );
	
	for( i = 0; i < 5; ++i ) {
		printf( "this is a test %d\n", i );
		sleep(1);
	}
		
	return 0;
}
