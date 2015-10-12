#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <asm/uaccess.h>	// for copy _user

MODULE_LICENSE( "Dual BSD/GPL" );

#define PROC_ENTRY_NAME "SleepDemo"
#define BUFLEN 1023

static struct proc_dir_entry *my_proc_entry = 0;
static char buf[BUFLEN+1];
static int cont = 1;
static unsigned int count = 0;
static DECLARE_WAIT_QUEUE_HEAD(wq);
static int flag = 0;

static ssize_t entry_read(char *buffer,
	      char **buffer_location,
	      off_t offset, int count, int *eof, void *data)
{	
	printk(KERN_INFO "process %i (%s) going to sleep\n",
			current->pid, current->comm);
//	wait_event_interruptible_timeout(wq, flag != 0, 5*HZ); //!! no need to plus jiffies
	wait_event_interruptible(wq, flag != 0);
	flag = 0;
	printk(KERN_INFO "awoken %i (%s)\n", current->pid, current->comm);
	return 0; /* EOF */
}

static ssize_t entry_write(struct file *file, const char *buffer, unsigned long count,
		   void *data)
{
	printk(KERN_INFO "process %i (%s) awakening the readers...\n",
			current->pid, current->comm);
	flag = 1;
	wake_up_interruptible(&wq);
	return count; /* succeed, to avoid retrial */
}


int init_module(void)
{
	printk( "module procfs_demo installed!\n" );
	
	memset( buf, 0, sizeof(buf) );
	
	my_proc_entry = create_proc_entry( PROC_ENTRY_NAME, 0644, NULL );
	if( !my_proc_entry ) {
		printk( "cannot create proc file entry!\n" );
		remove_proc_entry(PROC_ENTRY_NAME, NULL);
		return -ENOMEM;
	}
	
	my_proc_entry->read_proc  = entry_read;
	my_proc_entry->write_proc = entry_write;
	my_proc_entry->uid = 0;	// root user
	my_proc_entry->gid = 0; // root group
	
	return 0;
}

void cleanup_module(void)
{
	remove_proc_entry( PROC_ENTRY_NAME, NULL );
	printk( "module procfs_demo removed!\n" );
}













