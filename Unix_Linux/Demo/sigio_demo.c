#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>


const char *cmdStr = "./pty ./print";
static char	line[1024];
static int 	fd1[2], fd2[2];


int
set_fl(int fd, int flags) /* flags are file status flags to turn on */
{
	int		val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		return -1;
	val |= flags;		/* turn on flags */

	if (fcntl(fd, F_SETFL, val) < 0)
		return -1;

	return 0;
}


static void
sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	exit(1);
}


static void
sig_io(int signo)
{
	ssize_t nRead;

	nRead = read(fd2[0], line, 1024);
	if( nRead > 0 ) {
		line[nRead] = 0;
		printf( "%s", line );
	} else {
		printf( "read error, %d %s", nRead, strerror(errno) );
	}
}


int
main(void)
{
	pid_t	pid;
	int 	n;
	
	if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
		exit(-1);

	if (pipe(fd1) < 0 || pipe(fd2) < 0)
		exit(-1);

	fcntl(fd2[0], F_SETOWN, getpid());
	ioctl(fd2[0], FIOASYNC, &n);
//	set_fl(fd2[0], O_NONBLOCK);			//!! not work for async IO
	signal( SIGIO, sig_io );

	if ((pid = fork()) < 0) {
		exit(-1);
	} else if (pid > 0) {							/* parent */
		close(fd1[0]);
		close(fd2[1]);

		for( ; ; )
			pause();
	} else {									/* child */
		close(fd1[1]);
		close(fd2[0]);
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
				exit(-1);
			close(fd1[0]);
		}

		if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
				exit(-1);
			close(fd2[1]);
		}
		if (execl("/bin/sh", "sh", "-c", cmdStr, (char *)0) < 0)
			exit(-1);
	}
	exit(0);
}



