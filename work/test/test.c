#include <stdio.h>

struct Test {
	int a :2;
	int b :2;
	int c :1;
};

int main()
{
	struct Test t;
	
	t.a = 1;
	t.b = 3;
	t.c = 1;

	printf( "%d\n", sizeof(t) );
	printf( "%d %d %d\n", t.a, t.b, t.c );
	return 0;
}
