#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>


int atoi( const char *str )
{
	static const char *SPACES = " \t\f\r\v\n";
	int retval = 0;
	int newValue = 0;
	int negtive = 0;
	
	// 判断输入是否合法
	if( !str || !(*str) )
		goto ERROR;
	// skip leading spaces
	str += strspn( str, SPACES );
	// 再判断是否合法
	if( !(*str) )		// ________
		goto ERROR;
	
	if( *str == '+' ) ++str;
	else if( *str == '-' ) {
		negtive = 1;
		++str;
	} else if( !isdigit(*str) ) // a ___a
		goto ERROR;
	
	// +\0 +a
	if( !(*str) || !isdigit(*str) )
		goto ERROR;
	
	// 遇到非法字符就结束循环
	for( ; *str && isdigit(*str); ++str ) {
		newValue = retval + retval * 10 + (*str - '0');
		if( newValue < retval )
			goto OVERFLOW;
		retval = newValue;
	}
		
	
	// 不要忘记之前定义的negative
	if( negative )
		retval *= -1;
	
	return retval;
ERROR:
	errno = EINVAL;
	return 0;
OVERFLOW:
	errno = ERANGE;
	return 0;
}

















