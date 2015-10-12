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
#include <asm/uaccess.h>	/* for put_user */

#define DEVICE_NAME "chardev"	/* Dev name as it appears in /proc/devices   */

MODULE_LICENSE( "Dual BSD/GPL" );

static dev_t devno;
static struct cdev *mydev = 0;
static char *dev_buf = 0;
static int dev_buf_len = 1024;
static char *envp[] = { "HOME=/",		// for run user cmd
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
 
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

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
	
	call_usermodehelper( argv[0], argv, envp, UMH_WAIT_PROC );
	
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
	
	call_usermodehelper( argv[0], argv, envp, UMH_WAIT_PROC );
		
	for( i = 0; i < 3; ++i )
		kfree( argv[i] );
}


int init_module(void)
{
	int err;
	
	remove_dev();
	
	dev_buf = kmalloc( dev_buf_len, GFP_KERNEL );
	if( !dev_buf ) {
		printk( "cannot allocate memory!\n" );
		return -ENOMEM;
	}
	
	memset( dev_buf, 0, dev_buf_len );
	
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

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	kfree( dev_buf );
	cdev_del( mydev );
	unregister_chrdev_region( devno, 1 );
	remove_dev();
	printk(KERN_ALERT "unregister_chrdev finished\n");
}

/*
 * Methods
 */

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
	printk( "Opening device!\n" );
	return 0;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	printk( "releasing device!\n" );
	return 0;
}

static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char __user *buffer,	/* buffer to fill with data */
			   size_t count,	/* length of the buffer     */
			   loff_t *offset)
{
	printk( "device_read: count=%d, offset=%d\n", count, *offset );

	if( *offset >= dev_buf_len )
		return 0;
		
	if( *offset + count > dev_buf_len )
		count = dev_buf_len - *offset;
		
	if( copy_to_user(buffer, dev_buf + *offset, count) )
		return -EFAULT;
	
	*offset += count;
	
	return count;
}

static ssize_t
device_write(struct file *filp, const char __user *buffer, size_t count, loff_t * offset)
{
	printk( "device_write: count=%d, offset=%d\n", count, *offset );

	if( *offset >= dev_buf_len )
		return 0;
		
	if( *offset + count > dev_buf_len )
		count = dev_buf_len - *offset;
		
	if( copy_from_user( dev_buf + *offset, buffer, count) )
		return -EFAULT;
	
	*offset += count;
	
	return count;
}














