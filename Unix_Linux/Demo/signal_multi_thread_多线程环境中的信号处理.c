#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>


/*
本次实验结果：只有主进程收到信号，子进程通过对信号的解除block不起作用。
依据apue中的example： 专用信号处理线程实现：
首先要在主线程中用 pthread_sigmask BLOCK 要处理的信号。
在信号处理线程中无限循环调用 sigwait 等待信号
*/


typedef	void	Sigfunc(int);	/* for signal handlers */


Sigfunc *
signal_interrupt(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_flags |= SA_INTERRUPT;		/* SVR4, 44BSD */
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}



void sig_handler( int signo )
{ printf("received signal %d on thread %ld\n", signo, (long)pthread_self()); }


void *
thr_fn(void *arg)
{
	printf("Thread %ld started...\n", (long)pthread_self());
	sleep(10);
	return((void *)0);
}

void *
thr_fn1(void *arg)
{
	sigset_t newmask;
	sigemptyset(&newmask);
	
	pthread_sigmask( SIG_SETMASK, &newmask, NULL );
	
	printf("Thread %ld started...\n", (long)pthread_self());
	sleep(10);
	return((void *)0);
}

void test()
{
	pthread_t thread_group[10];
	int i, err;
	sigset_t newmask;
	
	sigfillset(&newmask);
	pthread_sigmask(SIG_BLOCK, &newmask, NULL);
	
	signal( SIGINT, sig_handler );
	// signal_interrupt( SIGINT, sig_handler );
	
	for( i = 0; i < 10; ++i ) {
		if( (err = pthread_create(&thread_group[i], NULL, thr_fn, NULL)) < 0 ) {
			printf("create thread error %s\n", strerror(err));
			exit(-1);
		} // if
	} // for
	
	for( i = 0; i < 10; ++i ) {
		if( (err = pthread_join(thread_group[i], NULL)) < 0 )
			printf("join thread error %s\n", strerror(err));
		else
			printf("thread %ld ended.\n", (long)thread_group[i]);
	} // for
}


int main()
{
	printf("program started... thread = %ld\n", (long)pthread_self());
	test();
	printf("program end!\n");

	return 0;
}  


//!! 默认只有主线程收到信号 pthread_join restart 貌似pthread_join实现对EINTR做了处理
/* 
program started... thread = 139845037426496
Thread 139844978841344 started...
Thread 139844970448640 started...
Thread 139844962055936 started...
Thread 139844953663232 started...
Thread 139844987234048 started...
Thread 139844995626752 started...
Thread 139845004019456 started...
Thread 139845012412160 started...
Thread 139845020804864 started...
Thread 139845029197568 started...
received signal 2 on thread 139845037426496
received signal 2 on thread 139845037426496
received signal 2 on thread 139845037426496
received signal 2 on thread 139845037426496
received signal 2 on thread 139845037426496
received signal 2 on thread 139845037426496
thread 139845029197568 ended.
thread 139845020804864 ended.
thread 139845012412160 ended.
thread 139845004019456 ended.
thread 139844995626752 ended.
thread 139844987234048 ended.
thread 139844978841344 ended.
thread 139844970448640 ended.
thread 139844962055936 ended.
thread 139844953663232 ended.
program end!
 */







