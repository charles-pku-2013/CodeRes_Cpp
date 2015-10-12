#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

void test( int no, const char *fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf( stderr, fmt, ap );	//!! v打头版本printf接受 va_arg 参数
	va_end(ap);
}


int main( int argc, char **argv )
{
	int i = 10;
	test(i, "%d\n", i);
	
	return 0;
}


