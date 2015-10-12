#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/uaccess.h>	// for copy _user

MODULE_LICENSE( "Dual BSD/GPL" );

#define PROC_ENTRY_NAME "ProcDemo"
#define BUFLEN 1023

static struct proc_dir_entry *my_proc_entry = 0;
static char buf[BUFLEN+1];

static int entry_open(struct inode *inode, struct file *filp)
{
	printk( "openning proc file %s\n", PROC_ENTRY_NAME );
	return 0;
}

static int entry_release(struct inode *inode, struct file *file)
{
	printk( "releasing proc file %s\n", PROC_ENTRY_NAME );
	return 0;
}

static ssize_t entry_read(struct file *filp,	/* see include/linux/fs.h   */
			   char __user *buffer,	/* buffer to fill with data */
			   size_t count,	/* length of the buffer     */
			   loff_t * offset)
{
	int i;
	
	printk( "entry_read() count=%d, offset=%d\n", count, *offset );
	
	//!! without this, there will be endless output.
	if( *offset > 0 )
		return 0;
	
	int buflen = strlen(buf);
	
	if( count > buflen )
		count = buflen;
	
	for( i = 0; i < count; ++i )
		put_user( buf[i], buffer+i );
		
	*offset += i;
	
	return i;
}

static ssize_t entry_write(struct file *filp,	/* see include/linux/fs.h   */
			   const char __user *buffer,	/* buffer to fill with data */
			   size_t count,	/* length of the buffer     */
			   loff_t * offset)
{
	int i;
	
	printk( "entry_write() count=%d offset=%d\n", count, *offset );

	if( count > BUFLEN )
		count = BUFLEN;
	
	for( i = 0; i < count; ++i )
		get_user( buf[i], buffer+i );
	buf[i] = 0;
	
	*offset += i;
	
	printk( "data in buf is %s\n", buf );
	
	return i;
}


int entry_permission (struct inode *inode, int op )
{
/*
	if( 2 == op || 4 == op ) // read & write for root
		return 0;
	return -EACCES;
*/
	return 0;
}

static struct file_operations file_ops = {
	.owner = THIS_MODULE,
	.open = entry_open,
	.release = entry_release,
	.read = entry_read,
	.write = entry_write,
};

static struct inode_operations inode_ops = {
	.permission = entry_permission,
};

int init_module(void)
{
	printk( "module procfs_demo installed!\n" );
	
	memset( buf, 0, sizeof(buf) );
	
	my_proc_entry = create_proc_entry( PROC_ENTRY_NAME, 0644, NULL );
	if( !my_proc_entry ) {
		printk( "cannot create proc file entry!\n" );
		return -ENOMEM;
	}
	
	my_proc_entry->proc_fops = &file_ops;
	my_proc_entry->proc_iops = &inode_ops;
	my_proc_entry->uid = 0;	// root user
	my_proc_entry->gid = 0; // root group
	my_proc_entry->size = 10;
	
	return 0;
}

void cleanup_module(void)
{
	remove_proc_entry( PROC_ENTRY_NAME, NULL );
	printk( "module procfs_demo removed!\n" );
}













