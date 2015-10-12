#include <cstdio>

void test_func()
{
	static int static_count = 0;
	int auto_count = 0;
	printf("static_count = %d, auto_count = %d\n", ++static_count, ++auto_count);
}

int main()
{
	test_func();
	test_func();
	test_func();
	test_func();
	test_func();
	return 0;
}