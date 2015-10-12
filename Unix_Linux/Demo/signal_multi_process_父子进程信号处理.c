#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>


void sig_handler( int signo )
{ printf("received signal %d on process %d\n", signo, (int)getpid()); }


void test1()
{
	pid_t pid;
	int status;
	
	signal( SIGINT, sig_handler );
	
	if( (pid = fork()) < 0 ) {
		printf("fork error!\n");
		exit(-1);
	} // if
	
	//!! 子进程确实继承了父进程的信号处理函数
	if( pid == 0 ) {
		//!! 实验中，信号发给子进程 
		sigset_t newmask;
		sigfillset( &newmask );
		sigprocmask(SIG_BLOCK, &newmask, NULL);
		
		printf("child process %d\n", (int)getpid());
		sleep(10);
		_exit(0);
	}
	
	if(waitpid( pid, &status, 0 ) < 0)
		perror("waitpid error!");
}

//!! 所有的子进程都收到了信号并执行了信号处理函数，但父进程没有
void test2()
{
	pid_t process_group[10], pid;
	int status, i;
	
	signal( SIGINT, sig_handler );
	
	for( i = 0; i < 10; ++i ) {
		if( (process_group[i] = fork()) < 0 ) {
			printf("fork error!\n");
			exit(-1);
		} // if
		
		if( process_group[i] == 0 ) {		
			printf("child process %d\n", (int)getpid());
			sleep(10);
			_exit(0);
		} // if		
	} // for
/* 	
	for( i = 0; i < 10; ++i ) {
		//!! waitpid内部实现应该是屏蔽了信号，所以只看到子进程们收到信号。
		if( (pid = waitpid( process_group[i], &status, 0 )) < 0)
			perror("waitpid error!");
		else
			printf("child process %d ended.\n", (int)pid);
	} // for
	 */
	
	//!! 改成sleep，信号到达所有进程，包括主进程
	sleep(15);
}


int main ()
{
	printf("program started... pid = %d\n", (int)getpid());
	test2();
	printf("program end!\n");

	return 0;
}  



/* 
program started... pid = 4646
child process 4653
child process 4654
child process 4652
child process 4655
child process 4651
child process 4656
child process 4650
child process 4649
child process 4648
child process 4647
received signal 2 on process 4656
received signal 2 on process 4655
received signal 2 on process 4654
received signal 2 on process 4653
received signal 2 on process 4652
received signal 2 on process 4651
received signal 2 on process 4650
received signal 2 on process 4649
received signal 2 on process 4648
received signal 2 on process 4647
child process 4647 ended.
child process 4648 ended.
child process 4649 ended.
child process 4650 ended.
child process 4651 ended.
child process 4652 ended.
child process 4653 ended.
child process 4654 ended.
child process 4655 ended.
child process 4656 ended.
program end!
 */

//!! waitpid 改成 sleep
/* 
program started... pid = 4694
child process 4701
child process 4700
child process 4702
child process 4703
child process 4699
child process 4704
child process 4698
child process 4697
child process 4696
child process 4695
received signal 2 on process 4704
received signal 2 on process 4703
received signal 2 on process 4702
received signal 2 on process 4701
received signal 2 on process 4700
received signal 2 on process 4699
received signal 2 on process 4698
received signal 2 on process 4697
received signal 2 on process 4696
received signal 2 on process 4695
received signal 2 on process 4694
program end!
 */









