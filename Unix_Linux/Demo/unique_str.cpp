#include <cstdio>
#include <set>
#include <string>
#include <cstring>

static
char *unique( char *str )
{
	typedef std::set<char> CharSet;
	typedef std::pair<CharSet::iterator, bool> InsertRetType;

	if( !str || !(*str) )
		return str;

	CharSet charset;
	char *end = str + strlen(str);

	for( char *p = end - 1; p >= str; --p ) {
		InsertRetType ret = charset.insert( *p );
		if( !ret.second ) {
			memmove( p, p+1, end - p );
			--end;
		} // if
	} // for

	return str;
}

int main()
{
//	freopen( "input.txt", "r", stdin );

	char str[256] = "abababab";
//	fgets( str, 256, stdin );
//	str[ strlen[str]-1 ] = 0;

	unique( str );

	printf( "%s\n", str );
	
	return 0;
}

