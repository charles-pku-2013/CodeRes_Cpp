#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <linux/kmod.h>
#include <asm/uaccess.h>	// for copy _user


MODULE_LICENSE( "Dual BSD/GPL" );

#define BUFLEN 1024
#define PROC_ENTRY_NAME "runcmd"

static char *envp[] = { "HOME=/",		// for run user cmd
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

static struct proc_dir_entry *my_proc_entry = 0;

static void call_user_cmd( const char *arg )
{
	char *argv[20];
	char buf[20];
	char *p = arg, *q;
	int count, len, i;

	if( !arg )
		return;

	count = 0;
	do {
		p += strspn( p, " \t\f\r\v\n" );
		q = p + strcspn( p, " \t\f\r\v\n" );
		if( p != q ) {
			len = q - p;
			argv[count] = kmalloc( len+1, GFP_KERNEL );
			memcpy( argv[count], p, len );
			argv[count][len] = 0;
			++count;
		}
		p = q;
	} while( *p && *q );
	argv[count] = 0;

//dbg
	printk( "runcmd:\n" );
	for( i = 0; i < count; ++i )
		printk( "%s ", argv[i] );
	printk( "\n" );
//dbg

	if( call_usermodehelper( argv[0], argv, envp, UMH_WAIT_PROC ) )
		printk( "cannot run user space cmd: %s!\n", arg );	

	for( i = 0; i < count; ++i )
		kfree( argv[i] );
}

static ssize_t entry_write(struct file *file, const char *buffer, unsigned long count,
		   void *data)
{	
//	printk( "entry_write() count=%d\n", count );
	char buf[BUFLEN];

	if( count > BUFLEN - 1 )
		count = BUFLEN - 1;

	/* write data to the buffer */
	if ( copy_from_user(buf, buffer, count) ) {
		return -EFAULT;
	}
	
	buf[count] = 0;
	if( isspace(buf[strlen(buf)-1]) )
		buf[strlen(buf)-1] = 0;				//!!

	printk( "received user cmd: %s\n", buf );

	call_user_cmd( buf );

	return count;
}


int init_module(void)
{
	printk( "module runcmd installed!\n" );
		
	my_proc_entry = create_proc_entry( PROC_ENTRY_NAME, 0644, NULL );
	if( !my_proc_entry ) {
		printk( "cannot create proc file entry!\n" );
		remove_proc_entry(PROC_ENTRY_NAME, NULL);
		return -ENOMEM;
	}
	
//	my_proc_entry->read_proc  = entry_read;
	my_proc_entry->write_proc = entry_write;
	my_proc_entry->uid = 0;	// root user
	my_proc_entry->gid = 0; // root group
	
	return 0;
}

void cleanup_module(void)
{
	remove_proc_entry( PROC_ENTRY_NAME, NULL );
	printk( "module runcmd removed!\n" );
}













