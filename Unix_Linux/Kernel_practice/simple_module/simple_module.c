#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h> 		// use current pointer to refer the running process.
#include <linux/errno.h>
#include <linux/moduleparam.h>

MODULE_LICENSE( "Dual BSD/GPL" );

static int my_init(void)
{
	printk( "Hello, simple module\n" );
	printk( "The current process is %s pid is %d\n", current->comm, current->pid );
	return 0;
}

static void my_exit(void)
{
	printk( "Goodbye! simple_module\n" );
}

module_init( my_init );
module_exit( my_exit );
