#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE     1024


typedef void    Sigfunc(int);   /* for signal handlers */

Sigfunc *
Signal(int signo, Sigfunc *func)
{
    struct sigaction    act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef  SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;   /* SunOS 4.x */
#endif
    } else {
#ifdef  SA_RESTART
        act.sa_flags |= SA_RESTART;     /* SVR4, 44BSD */
#endif
    }
    if (sigaction(signo, &act, &oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}


Sigfunc *
signal_restart(int signo, Sigfunc *func)
{
    struct sigaction    act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_flags |= SA_RESTART;     /* SVR4, 44BSD */
    if (sigaction(signo, &act, &oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}


// SA_INTERRUPT MacOS不支持
Sigfunc *
signal_interrupt(int signo, Sigfunc *func)
{
    struct sigaction    act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_flags |= SA_INTERRUPT;       /* SVR4, 44BSD */
    if (sigaction(signo, &act, &oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}


void sig_handler( int signo )
{ printf("received signal %d on process %d\n", signo, (int)getpid()); }


void test1()
{
    char buf[BUFSIZE];
    
    signal( SIGINT, sig_handler );
    
    sleep(10);                              // interrupt    SA_RESTART doesn't work
    // read(STDIN_FILENO, buf, BUFSIZE);        // restart
    // pause();                                 // interrupt    SA_RESTART doesn't work
}


void test2()
{
    char buf[BUFSIZE];
    
    if( signal_restart( SIGINT, sig_handler ) == SIG_ERR ) {
        printf("failed to install signal handler!\n");
        exit(-1);
    } // if
    
    // sleep(10);                               // interrupt
    // read(STDIN_FILENO, buf, BUFSIZE);        // restart
    pause();                                    // interrupt

    printf("errno = %s\n", strerror(errno));
}


void test3()
{
    char buf[BUFSIZE];
    ssize_t n;
    
    if( signal_interrupt( SIGINT, sig_handler ) == SIG_ERR ) {
        printf("failed to install signal handler!\n");
        exit(-1);
    } // if
    
    /* sleep(10);                               // interrupt */
    n = read(STDIN_FILENO, buf, BUFSIZE);        // interrupt //!! different from normal behavior
    printf( "%zd\n", n );
    /* pause();                                    // interrupt */

    printf("errno = %s\n", strerror(errno));
}



int main ()
{
    printf("program started... pid = %d\n", (int)getpid());
    test3();
    printf("program end!\n");

    return 0;
}  











