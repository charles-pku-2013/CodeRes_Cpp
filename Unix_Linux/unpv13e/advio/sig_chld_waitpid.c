#include	"unp.h"

void
sig_chld(int signo)
{
	pid_t	pid;
	int		stat;
	//!! 用循环回收僵尸子进程 标准 SIGCHLD 处理模板
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0) {
		printf("child %d terminated\n", pid);
	}
	return;
}
