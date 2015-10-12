#include "pty.h"



int
posix_openpt(int oflag)
{
	int		fdm;

	fdm = open("/dev/ptmx", oflag);
	return(fdm);
}

int
ptym_open(char *pts_name, int pts_namesz)
{
	char	*ptr;
	int		fdm;

	/*
	 * Return the name of the master device so that on failure
	 * the caller can print an error message.  Null terminate
	 * to handle case where string length > pts_namesz.
	 */
	strncpy(pts_name, "/dev/ptmx", pts_namesz);
	pts_name[pts_namesz - 1] = '\0';

	fdm = posix_openpt(O_RDWR);
	if (fdm < 0)
		return(-1);
	if (grantpt(fdm) < 0) {		/* grant access to slave */
		close(fdm);
		return(-2);
	}
	if (unlockpt(fdm) < 0) {	/* clear slave's lock flag */
		close(fdm);
		return(-3);
	}
	if ((ptr = (char*)ptsname(fdm)) == NULL) {	/* get slave's name */
		close(fdm);
		return(-4);
	}

	/*
	 * Return name of slave.  Null terminate to handle case
	 * where strlen(ptr) > pts_namesz.
	 */
	strncpy(pts_name, ptr, pts_namesz);
	pts_name[pts_namesz - 1] = '\0';
	return(fdm);			/* return fd of master */
}

int
ptys_open(char *pts_name)
{
	int fds;

	if ((fds = open(pts_name, O_RDWR)) < 0)
		return(-5);
	return(fds);
}


pid_t
pty_fork(int *ptrfdm, char *slave_name, int slave_namesz,
		 const struct termios *slave_termios,
		 const struct winsize *slave_winsize)
{
	int		fdm, fds;
	pid_t	pid;
	char	pts_name[20];

	if ((fdm = ptym_open(pts_name, sizeof(pts_name))) < 0) {
		err_sys("can't open master pty: %s, error %d", pts_name, fdm);
		return -1;
	}

	if (slave_name != NULL) {
		/*
		 * Return name of slave.  Null terminate to handle case
		 * where strlen(pts_name) > slave_namesz.
		 */
		strncpy(slave_name, pts_name, slave_namesz);
		slave_name[slave_namesz - 1] = '\0';
	}

	if ((pid = fork()) < 0) {
		return(-1);
	} else if (pid == 0) {		/* child */
		if (setsid() < 0) {
			err_sys("setsid error");
			return -1;
		} // if setsid

		/*
		 * System V acquires controlling terminal on open().
		 */
		if ((fds = ptys_open(pts_name)) < 0) {
			err_sys("can't open slave pty");
			return -1;
		}
		close(fdm);		/* all done with master in child */

#if	defined(TIOCSCTTY)
		/*
		 * TIOCSCTTY is the BSD way to acquire a controlling terminal.
		 */
		if (ioctl(fds, TIOCSCTTY, (char *)0) < 0) {
			err_sys("TIOCSCTTY error");
			return -1;
		}
#endif
		/*
		 * Set slave's termios and window size.
		 */
		if (slave_termios != NULL) {
			if (tcsetattr(fds, TCSANOW, slave_termios) < 0) {
				err_sys("tcsetattr error on slave pty");
				return -1;
			}
		}
		if (slave_winsize != NULL) {
			if (ioctl(fds, TIOCSWINSZ, slave_winsize) < 0) {
				err_sys("TIOCSWINSZ error on slave pty");
				return -1;
			}
		}

		/*
		 * Slave becomes stdin/stdout/stderr of child.
		 */
		if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
			return -1;
		if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
			return -1;
		if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)
			return -1;
		if (fds != STDIN_FILENO && fds != STDOUT_FILENO &&
		  fds != STDERR_FILENO)
			close(fds);
		return(0);		/* child returns 0 just like fork() */
	} else {					/* parent */
		*ptrfdm = fdm;	/* return fd of master */
		return(pid);	/* parent returns pid of child */
	}
}


static void
set_noecho(int fd)		/* turn off echo (for slave pty) */
{
	struct termios	stermios;

	if (tcgetattr(fd, &stermios) < 0)
		err_sys("tcgetattr error");

	stermios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);

	/*
	 * Also turn off NL to CR/NL mapping on output.
	 */
	stermios.c_oflag &= ~(ONLCR);

	if (tcsetattr(fd, TCSANOW, &stermios) < 0)
		err_sys("tcsetattr error");
}



int
main(int argc, char *argv[])
{
	int				fdm, c, ignoreeof, interactive, noecho, verbose;
	pid_t			pid;
	char			*driver;
	char			slave_name[20];
	struct termios	orig_termios;
	struct winsize	size;

	interactive = isatty(STDIN_FILENO);
	ignoreeof = 0;
	noecho = 0;
	verbose = 0;
	driver = NULL;

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ((c = getopt(argc, argv, OPTSTR)) != EOF) {
		switch (c) {
		case 'd':		/* driver for stdin/stdout */
			driver = optarg;
			break;

		case 'e':		/* noecho for slave pty's line discipline */
			noecho = 1;
			break;

		case 'i':		/* ignore EOF on standard input */
			ignoreeof = 1;
			break;

		case 'n':		/* not interactive */
			interactive = 0;
			break;

		case 'v':		/* verbose */
			verbose = 1;
			break;

		case '?':
			err_quit("unrecognized option: -%c", optopt);
		}
	}
	if (optind >= argc)
		err_quit("usage: pty [ -d driver -einv ] program [ arg ... ]");

	if (interactive) {	/* fetch current termios and window size */
		if (tcgetattr(STDIN_FILENO, &orig_termios) < 0)
			err_sys("tcgetattr error on stdin");
		if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *) &size) < 0)
			err_sys("TIOCGWINSZ error");
		pid = pty_fork(&fdm, slave_name, sizeof(slave_name),
		  &orig_termios, &size);
	} else {
		pid = pty_fork(&fdm, slave_name, sizeof(slave_name),
		  NULL, NULL);
	}

	if (pid < 0) {
		err_sys("fork error");
	} else if (pid == 0) {		/* child */
		if (noecho)
			set_noecho(STDIN_FILENO);	/* stdin is slave pty */

		if (execvp(argv[optind], &argv[optind]) < 0)
			err_sys("can't execute: %s", argv[optind]);
	}

	if (verbose) {
		fprintf(stderr, "slave name = %s\n", slave_name);
		if (driver != NULL)
			fprintf(stderr, "driver = %s\n", driver);
	}

	if (interactive && driver == NULL) {
		if (tty_raw(STDIN_FILENO) < 0)	/* user's tty to raw mode */
			err_sys("tty_raw error");
		if (atexit(tty_atexit) < 0)		/* reset user's tty on exit */
			err_sys("atexit error");
	}

	if (driver)
		do_driver(driver);	/* changes our stdin/stdout */

	loop(fdm, ignoreeof);	/* copies stdin -> ptym, ptym -> stdout */

	exit(0);
}






//int main()
//{
//	pid_t pid;
//	char buf[256];
//	int fd;
//	ssize_t n;

//	pid = pty_fork(&fd, buf, 256, NULL, NULL);
//	if( pid < 0 ) {
//		printf( "create process error!\n" );
//		exit( -1 );
//	}

//	printf( "slave pty name is: %s\n", buf );
//	if( 0 == pid ) {
//		printf( "child process going to run execl\n" );
//		if (execl("/bin/sh", "sh", "-c", "./child", (char *)0) < 0)
//			exit(-1);		
//	} // child

//	printf( "parent process testing:\n" );
//	while( fgets(buf, 256, stdin) ) {
//		write( fd, buf, strlen(buf)+1 );
//		n = read( fd, buf, 256 );
//		if( n <= 0 ) {
//			printf( "parent read error!\n" );
//			continue;
//		}
//		write( STDOUT_FILENO, buf, n );
//	}
//	
//	return 0;
//}

