#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int type;
	char str[0];
}Test;

int main()
{
	Test *test;
	printf( "%u\n", sizeof(*test) );

	test = (Test*)malloc(sizeof(Test)+256);

	test->type = 10;
	sprintf( test->str, "Hello, world!" );

	printf( "%d: %s\n", test->type, test->str );

	free(test);
	
	return 0;
}