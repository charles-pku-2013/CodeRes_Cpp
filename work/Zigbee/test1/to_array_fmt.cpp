#include <string>
#include <cstdio>
#include <cstring>

int main()
{
	using namespace std;
	
	char str[] = "0200098312346174696E666Fxx03";
	
	int i = 0, j = 2;
	do {
		string s( "0x" );
		s.append( str+i, str+j );
		printf( "%s, ", s.c_str() );
		i = j; j += 2;
	} while( i < strlen(str) );
	putchar( '\n' );
	
	return 0;
}
