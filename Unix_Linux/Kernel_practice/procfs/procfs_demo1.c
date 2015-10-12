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


static ssize_t entry_read(char *buffer,
	      char **buffer_location,
	      off_t offset, int count, int *eof, void *data)
{	
	printk( "entry_read() count=%d, offset=%d\n", count, offset );

	//!! give all info at one time.
	if( offset > 0 )
		return 0;

	int len = strlen(buf);
	
	//!! this is enough
	*buffer_location = buf;
//	memcpy( buffer, buf, len );
//	copy_to_user( buffer, buf, len );	//!! copy_to_user cannot be used here, the reason is unknown
	
	return len;
}

static ssize_t entry_write(struct file *file, const char *buffer, unsigned long count,
		   void *data)
{	
	printk( "entry_write() count=%d\n", count );

	if( count > BUFLEN )
		count = BUFLEN;

	/* write data to the buffer */
	if ( copy_from_user(buf, buffer, count) ) {
		return -EFAULT;
	}
	
	buf[count] = 0;
	
	printk( "data in buf is %s\n", buf );
	
	return count;
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













