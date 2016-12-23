#include <signal.h>
#include <stdio.h>
 
volatile sig_atomic_t gSignalStatus = 0;
 
void signal_handler(int signal)
{
    gSignalStatus = signal;
}
 
int main(void)
{
    /* Install a signal handler. */
    signal(SIGINT, signal_handler);
 
    printf("SignalValue:   %d\n", gSignalStatus);
    printf("Sending signal %d\n", SIGINT);
    raise(SIGINT);
    printf("SignalValue:   %d\n", gSignalStatus);
	
	return 0;
}