#ifndef __PTY_H
#define __PTY_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>



#define	BUFFSIZE	512
#define	MAXLINE	4096			/* max line length */
#define OPTSTR "+d:einv"


int		 tty_cbreak(int);				/* {Prog raw} */
int		 tty_raw(int);					/* {Prog raw} */
int		 tty_reset(int);				/* {Prog raw} */
void	 tty_atexit(void);				/* {Prog raw} */


ssize_t writen(int fd, const void *ptr, size_t n);
void do_driver(char *driver);
int s_pipe(int fd[2]);

typedef	void	Sigfunc(int);	/* for signal handlers */


#endif
