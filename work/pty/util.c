#include "pty.h"



int
s_pipe(int fd[2])
{
	return(socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
}


ssize_t             /* Write "n" bytes to a descriptor  */
writen(int fd, const void *ptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;

	nleft = n;
	while (nleft > 0) {
		if ((nwritten = write(fd, ptr, nleft)) < 0) {
			if (nleft == n)
				return(-1); /* error, return -1 */
			else
				break;      /* error, return amount written so far */
		} else if (nwritten == 0) {
			break;
		}
		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n - nleft);      /* return >= 0 */
}



Sigfunc *
signal_intr(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
#ifdef	SA_INTERRUPT
	act.sa_flags |= SA_INTERRUPT;
#endif
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}


void
do_driver(char *driver)
{
	pid_t	child;
	int		pipe[2];

	/*
	 * Create a stream pipe to communicate with the driver.
	 */
	if (s_pipe(pipe) < 0)
		err_sys("can't create stream pipe");

	if ((child = fork()) < 0) {
		err_sys("fork error");
	} else if (child == 0) {		/* child */
		close(pipe[1]);

		/* stdin for driver */
		if (dup2(pipe[0], STDIN_FILENO) != STDIN_FILENO)
			err_sys("dup2 error to stdin");

		/* stdout for driver */
		if (dup2(pipe[0], STDOUT_FILENO) != STDOUT_FILENO)
			err_sys("dup2 error to stdout");
		if (pipe[0] != STDIN_FILENO && pipe[0] != STDOUT_FILENO)
			close(pipe[0]);

		/* leave stderr for driver alone */
		execlp(driver, driver, (char *)0);
		err_sys("execlp error for: %s", driver);
	}

	close(pipe[0]);		/* parent */
	if (dup2(pipe[1], STDIN_FILENO) != STDIN_FILENO)
		err_sys("dup2 error to stdin");
	if (dup2(pipe[1], STDOUT_FILENO) != STDOUT_FILENO)
		err_sys("dup2 error to stdout");
	if (pipe[1] != STDIN_FILENO && pipe[1] != STDOUT_FILENO)
		close(pipe[1]);

	/*
	 * Parent returns, but with stdin and stdout connected
	 * to the driver.
	 */
}

