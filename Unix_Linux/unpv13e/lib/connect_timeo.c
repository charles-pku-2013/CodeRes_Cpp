/* include connect_timeo */
#include	"unp.h"
// ch 14.2 figure 14-1
static void	connect_alarm(int);

int
connect_timeo(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
	Sigfunc	*sigfunc;
	int		n;

	sigfunc = Signal(SIGALRM, connect_alarm);
	if (alarm(nsec) != 0)
		err_msg("connect_timeo: alarm was already set");
	//!! alarm 可能在这之间发生
	if ( (n = connect(sockfd, saptr, salen)) < 0) {
		//!! 如果类似connect的函数内部实现对EINTR处理是重启，则本程序技术无效
		close(sockfd);
		if (errno == EINTR)		//!! 当超时timeout发生，这样设置errno
			errno = ETIMEDOUT;
	}
	alarm(0);					/* turn off the alarm */
	Signal(SIGALRM, sigfunc);	/* restore previous signal handler */

	return(n);
}

static void
connect_alarm(int signo)
{
	return;		/* just interrupt the connect() */
}
/* end connect_timeo */

void
Connect_timeo(int fd, const SA *sa, socklen_t salen, int sec)
{
	if (connect_timeo(fd, sa, salen, sec) < 0)
		err_sys("connect_timeo error");
}
