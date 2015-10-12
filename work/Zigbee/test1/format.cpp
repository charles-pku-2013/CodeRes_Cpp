#include <string>
#include <cstdio>
#include <cstring>

void print_str( char *str )
{
	char buf[5];
	char *end = str + strlen(str);
	
	buf[4] = 0;
	strncpy( buf, "0x", 2 );
	for( ; str < end; str += 2 ) {
		strncpy( buf+2, str, 2 );
		printf( "%s, ", buf );
	}
	
	return;
}

int main()
{
	using namespace std;
	
	char str[] = "833d6174696e666faf";
	char *p;
	
	for( p = strtok(str, " []"); p; p = strtok(NULL, " []") )
		print_str( p );
	putchar( '\n' );	
	
	return 0;
}
