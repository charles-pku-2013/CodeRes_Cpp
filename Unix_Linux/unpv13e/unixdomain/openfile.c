#include	"unp.h"

int
main(int argc, char **argv)
{
	int		fd;

	if (argc != 4)
		err_quit("openfile <sockfd#> <filename> <mode>");

	if ( (fd = open(argv[2], atoi(argv[3]))) < 0)
		exit( (errno > 0) ? errno : 255 );
	//!! 给父进程发送描述符，从子进程向父进程传递描述符需要 msghdr textbook ch 15.7
	if (write_fd(atoi(argv[1]), "", 1, fd) < 0)
		exit( (errno > 0) ? errno : 255 );

	exit(0);
}
