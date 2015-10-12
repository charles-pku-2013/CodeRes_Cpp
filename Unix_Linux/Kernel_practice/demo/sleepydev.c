/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 !! to implement lseek
 ?? cannot append to file using echo >>
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kmod.h>
#include <linux/cdev.h>		// for struct cdev
#include <linux/wait.h>
#include <linux/sched.h>
#include <asm/uaccess.h>	/* for put_user */

#define DEVICE_NAME "sleepydev"	/* Dev name as it appears in /proc/devices   */

MODULE_LICENSE( "Dual BSD/GPL" );

static dev_t devno;
static struct cdev *mydev = 0;
static char *envp[] = { "HOME=/",		// for run user cmd
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
        
static DECLARE_WAIT_QUEUE_HEAD(wq);
static int flag = 0;

static void create_dev( int major, int minor )
{
	// mknod /dev/devicename c major minor
	char *argv[6];
	int i;
	argv[5] = NULL;
	for( i = 0; i < 4; ++i )
		argv[i] = kmalloc( 20, GFP_KERNEL );
	sprintf( argv[0], "/bin/mknod" );
	sprintf( argv[1], "/dev/%s", DEVICE_NAME );
	sprintf( argv[2], "c" );
	sprintf( argv[3], "%d", major );
	sprintf( argv[4], "%d", minor );
	
	if( call_usermodehelper( argv[0], argv, envp, UMH_WAIT_PROC ) )
		printk( "create device fail!\n" );
	
	for( i = 0; i < 4; ++i )
		kfree( argv[i] );
}

static void remove_dev()
{
	// /bin/rm -f /dev/devicename
	int i;
	char *argv[4];
	argv[3] = NULL;
	for( i = 0; i < 3; ++i )
		argv[i] = kmalloc( 20, GFP_KERNEL );
	sprintf( argv[0], "/bin/rm" );
	sprintf( argv[1], "-f" );
	sprintf( argv[2], "/dev/%s", DEVICE_NAME );
	
	if( call_usermodehelper( argv[0], argv, envp, UMH_WAIT_PROC ) )
		printk( "remove device fail!\n" );
		
	for( i = 0; i < 3; ++i )
		kfree( argv[i] );
}

static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char __user *buffer,	/* buffer to fill with data */
			   size_t count,	/* length of the buffer     */
			   loff_t *offset)
{
	printk(KERN_INFO "process %i (%s) going to sleep\n",
			current->pid, current->comm);
	wait_event_interruptible(wq, flag != 0);
	flag = 0;
	printk(KERN_INFO "awoken %i (%s)\n", current->pid, current->comm);
	return 0; /* EOF */
}

static ssize_t
device_write(struct file *filp, const char __user *buffer, size_t count, loff_t * offset)
{
	printk(KERN_INFO "process %i (%s) awakening the readers...\n",
			current->pid, current->comm);
	flag = 1;
	wake_up_interruptible(&wq);
	return count; /* succeed, to avoid retrial */
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
};

int init_module(void)
{
	int err;
	
	remove_dev();
	
	if( (err = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME)) < 0 ) {
		printk( "alloc_chrdev_region() error!\n" );
		return err;
	}
	
	if( (mydev = cdev_alloc()) == 0 ) {
		printk( "cdev_alloc() error\n" );
		return -1;
	}
	
	cdev_init( mydev, &fops );
	mydev->owner = THIS_MODULE;
	mydev->ops = &fops;
	
	if( (err = cdev_add( mydev, devno, 1 )) < 0 ) {
		printk( "cdev_add() fails\n" );
		return err;
	}

	printk(KERN_INFO "I was assigned major number %d, minor number %d. To talk to\n", MAJOR(devno), MINOR(devno));
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, MAJOR(devno));
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");
	
	create_dev( MAJOR(devno), MINOR(devno) );

	return 0;
}

void cleanup_module(void)
{
	cdev_del( mydev );
	unregister_chrdev_region( devno, 1 );
	remove_dev();
	printk(KERN_ALERT "unregister_chrdev finished\n");
}















