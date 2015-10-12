#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

const char *cmdStr = "fdisk /dev/sda";

static void
sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	exit(1);
}

int
main(void)
{
	int		n, fd1[2], fd2[2];
	pid_t	pid;
	char *cmd1 = "p\n";
	char *cmd2 = "q\n";

	if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
		exit(-1);

	if (pipe(fd1) < 0 || pipe(fd2) < 0)
		exit(-1);

	if ((pid = fork()) < 0) {
		exit(-1);
	} else if (pid > 0) {							/* parent */
		close(fd1[0]);
		close(fd2[1]);
		
		write( fd1[1], cmd1, 2 );
		sleep(2);
		write( fd1[1], cmd2, 2 );
	} else {									/* child */
		close(fd1[1]);
		close(fd2[0]);
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
				exit(-1);
			close(fd1[0]);
		}

#if 0
		if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
				err_sys("dup2 error to stdout");
			close(fd2[1]);
		}
#endif
		if (execl("/bin/sh", "sh", "-c", cmdStr, (char *)0) < 0)
			exit(-1);
	}
	exit(0);
}



