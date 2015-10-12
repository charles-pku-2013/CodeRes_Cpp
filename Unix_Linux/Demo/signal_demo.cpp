#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>

#define SIGMY		36		// SIGRTMIN+2

//!! 可用 kill -l 列出本系统支持的信号
//!! 实际使用中绝不限于系统定义的信号

void sig_int( int signo )
{ printf("Received signal INT\n"); }

void sig_my( int signo )
{ printf("Received signal MY\n"); }


int main()
{
	signal( SIGINT, sig_int );
	signal( SIGMY, sig_my );
	
	pause();
	
	return 0;
}