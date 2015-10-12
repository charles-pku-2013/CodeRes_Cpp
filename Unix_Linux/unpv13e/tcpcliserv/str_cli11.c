#include	"unp.h"
// textbook p114
void
str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];

	while (Fgets(sendline, MAXLINE, fp) != NULL) {
		Writen(sockfd, sendline, 1);	//!! 第一次写使server返回RST
		sleep(1);
		Writen(sockfd, sendline+1, strlen(sendline)-1);	//!! 第二次写，产生SIGPIPE

		if (Readline(sockfd, recvline, MAXLINE) == 0)
			err_quit("str_cli: server terminated prematurely");

		Fputs(recvline, stdout);
	}
}
