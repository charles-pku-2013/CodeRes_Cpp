#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

#define WORK_QUEUE_NAME "WorkQueue"

static void intrpt_routine( void* );

static struct workqueue_struct *work_queue = 0;
static struct work_struct task;
static DECLARE_WORK( task, intrpt_routine, NULL );

static unsigned int count = 0;
static int cont = 1;

static void intrpt_routine( void *args )
{
	printk( "intrpt_routine called for %u times.\n", ++count );
	if( cont )
		queue_delayed_work( work_queue, &task, 100 );
}

int init_module(void)
{
	printk( "module installed!\n" );
	work_queue = create_workqueue( WORK_QUEUE_NAME );
	queue_delayed_work( work_queue, &task, 100 );
}

void cleanup_module( void )
{
	printk( "module removed!\n" );
	cont = 0;
	cancel_delayed_work( &task );
	flush_workqueue( work_queue );
	destroy_workqueue( work_queue );
}
















