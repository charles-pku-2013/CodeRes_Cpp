#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>

int main()
{
	struct rlimit rl;
	
	getrlimit(RLIMIT_NOFILE, &rl);
	
	printf( "%ld, %ld\n", rl.rlim_cur, rl.rlim_max );
	
	rl.rlim_cur = 2048;
	setrlimit(RLIMIT_NOFILE, &rl);
	
	// in this case, OPEN_MAX greater than the size of fd_set, so we need to use poll instead of select
	printf( "%u\n", sizeof(fd_set) );
	printf( "%ld\n", sysconf( _SC_OPEN_MAX ) );
	return 0;
}
