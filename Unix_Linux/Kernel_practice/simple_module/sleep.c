#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h> 		// use current pointer to refer the running process.
#include <linux/errno.h>
#include <linux/moduleparam.h>

MODULE_LICENSE( "Dual BSD/GPL" );

static int cont = 1;
static unsigned int count = 0;

void my_sleep( int secs )
{
	unsigned long j;
	j = jiffies + secs * HZ;
	while( time_before(jiffies, j) )
		schedule();
}

static void func()
{
	while( cont ) {
		printk( "kernel thread running %u\n", ++count );
//		msleep_interruptible( 1000 );		//!! that will hang up the whole system??!!
		my_sleep(1);
	}
}

int init_module(void)
{
	printk( "Hello, simple module\n" );
	printk( "The current process is %s pid is %d\n", current->comm, current->pid );
	//!! cannot run long loop here, that will block the modprobe and even the whole system!!
	func();
	return 0;
}

void cleanup_module(void)
{
	cont = 0;
	printk( "Goodbye! simple_module\n" );
}

