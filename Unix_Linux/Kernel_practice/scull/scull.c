#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h> 		// use current pointer to refer the running process.
#include <linux/errno.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>		// struct file file_ops
#include <linux/cdev.h>		// for character device
#include <linux/malloc.h>   /* kmalloc() */
#include <linux/types.h>    /* size_t */

MODULE_LICENSE( "Dual BSD/GPL" );

static int scull_major = 0;
static Scull_Dev *scull_devices = 0; /* allocated in scull_init_module */
static int scull_nr_devs = 4;    /* number of bare scull devices */

typedef struct Scull_Dev {
   void **data;
   struct Scull_Dev *next;   /* next listitem */
   int quantum;              /* the current quantum size */
   int qset;                 /* the current array size */
   unsigned long size;
   unsigned int access_key;  /* used by sculluid and scullpriv */
   struct semaphore sem;     /* mutual exclusion semaphore     */
} Scull_Dev;

static Scull_Dev *scull_devices; /* allocated in scull_init_module */

static int scull_trim(Scull_Dev *dev)
{
    Scull_Dev *next, *dptr;
    int qset = dev->qset;   /* "dev" is not-null */
    int i;

    for (dptr = dev; dptr; dptr = next) { /* all the list items */
        if (dptr->data) {
            for (i = 0; i < qset; i++)
                if (dptr->data[i])
                    kfree(dptr->data[i]);
            kfree(dptr->data);
            dptr->data=NULL;
        }
        next=dptr->next;
        if (dptr != dev) kfree(dptr); /* all of them but the first */
    }
    dev->size = 0;
    dev->quantum = scull_quantum;
    dev->qset = scull_qset;
    dev->next = NULL;
    return 0;
}

/* In scull_open, the fop_array is used according to TYPE(dev) */
int scull_open(struct inode *inode, struct file *filp)
{
    Scull_Dev *dev; /* device information */
    int num = NUM(inode->i_rdev);
    int type = TYPE(inode->i_rdev);

    /*
     * the type and num values are only valid if we are not using devfs.
     * However, since we use them to retrieve the device pointer, we
     * don't need them with devfs as filp->private_data is already
     * initialized
     */

    /*
     * If private data is not valid, we are not using devfs
     * so use the type (from minor nr.) to select a new f_op
     */
    if (!filp->private_data && type) {
        if (type > SCULL_MAX_TYPE) return -ENODEV;
        filp->f_op = scull_fop_array[type];
        return filp->f_op->open(inode, filp); /* dispatch to specific open */
    }

    /* type 0, check the device number (unless private_data valid) */
    dev = (Scull_Dev *)filp->private_data;
    if (!dev) {
        if (num >= scull_nr_devs) return -ENODEV;
        dev = &scull_devices[num];
        filp->private_data = dev; /* for other methods */
    }

    /* now trim to 0 the length of the device if open was write-only */
    if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) {
        if (down_interruptible(&dev->sem)) {
            return -ERESTARTSYS;
        }
        scull_trim(dev); /* ignore errors */
        up(&dev->sem);
    }

    return 0;          /* success */
}

int scull_release(struct inode *inode, struct file *filp)
{
	printk( "scull_release() called\n" );
    return 0;
}
/*
 * Follow the list 
 */
static Scull_Dev *scull_follow(Scull_Dev *dev, int n)
{
    while (n--) {
        if (!dev->next) {
            dev->next = kmalloc(sizeof(Scull_Dev), GFP_KERNEL);
            memset(dev->next, 0, sizeof(Scull_Dev));
        }
        dev = dev->next;
        continue;
    }
    return dev;
}

/*
 * Data management: read and write
 */

ssize_t scull_read(struct file *filp, char *buf, size_t count,
                loff_t *f_pos)
{
    Scull_Dev *dev = filp->private_data; /* the first listitem */
    Scull_Dev *dptr;
    int quantum = dev->quantum;
    int qset = dev->qset;
    int itemsize = quantum * qset; /* how many bytes in the listitem */
    int item, s_pos, q_pos, rest;
    ssize_t ret = 0;

    if (down_interruptible(&dev->sem))
            return -ERESTARTSYS;
    if (*f_pos >= dev->size)
        goto out;
    if (*f_pos + count > dev->size)
        count = dev->size - *f_pos;
    /* find listitem, qset index, and offset in the quantum */
    item = (long)*f_pos / itemsize;
    rest = (long)*f_pos % itemsize;
    s_pos = rest / quantum; q_pos = rest % quantum;

    /* follow the list up to the right position (defined elsewhere) */
    dptr = scull_follow(dev, item);

    if (!dptr->data)
        goto out; /* don't fill holes */
    if (!dptr->data[s_pos])
        goto out;
    /* read only up to the end of this quantum */
    if (count > quantum - q_pos)
        count = quantum - q_pos;

    if (copy_to_user(buf, dptr->data[s_pos]+q_pos, count)) {
        ret = -EFAULT;
	goto out;
    }
    *f_pos += count;
    ret = count;

 out:
    up(&dev->sem);
    return ret;
}

ssize_t scull_write(struct file *filp, const char *buf, size_t count,
                loff_t *f_pos)
{
    Scull_Dev *dev = filp->private_data;
    Scull_Dev *dptr;
    int quantum = dev->quantum;
    int qset = dev->qset;
    int itemsize = quantum * qset;
    int item, s_pos, q_pos, rest;
    ssize_t ret = -ENOMEM; /* value used in "goto out" statements */

    if (down_interruptible(&dev->sem))
            return -ERESTARTSYS;

    /* find listitem, qset index and offset in the quantum */
    item = (long)*f_pos / itemsize;
    rest = (long)*f_pos % itemsize;
    s_pos = rest / quantum; q_pos = rest % quantum;

    /* follow the list up to the right position */
    dptr = scull_follow(dev, item);
    if (!dptr->data) {
        dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);
        if (!dptr->data)
            goto out;
        memset(dptr->data, 0, qset * sizeof(char *));
    }
    if (!dptr->data[s_pos]) {
        dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
        if (!dptr->data[s_pos])
            goto out;
    }
    /* write only up to the end of this quantum */
    if (count > quantum - q_pos)
        count = quantum - q_pos;

    if (copy_from_user(dptr->data[s_pos]+q_pos, buf, count)) {
        ret = -EFAULT;
	goto out;
    }
    *f_pos += count;
    ret = count;

    /* update the size */
    if (dev->size < *f_pos)
        dev-> size = *f_pos;

  out:
    up(&dev->sem);
    return ret;
}


/*
 * The "extended" operations -- only seek
 */
loff_t scull_llseek(struct file *filp, loff_t off, int whence)
{
    Scull_Dev *dev = filp->private_data;
    loff_t newpos;

    switch(whence) {
      case 0: /* SEEK_SET */
        newpos = off;
        break;

      case 1: /* SEEK_CUR */
        newpos = filp->f_pos + off;
        break;

      case 2: /* SEEK_END */
        newpos = dev->size + off;
        break;

      default: /* can't happen */
        return -EINVAL;
    }
    if (newpos<0) return -EINVAL;
    filp->f_pos = newpos;
    return newpos;
}

int scull_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
	printk( "unimplemented\n" );
	return 0;
}

struct file_operations scull_fops = {
	owner:		THIS_MODULE,
    llseek:     scull_llseek,
    read:       scull_read,
    write:      scull_write,
    ioctl:      scull_ioctl,
    open:       scull_open,
    release:    scull_release,
};


/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
void scull_cleanup_module(void)
{
    int i;
    
    unregister_chrdev(scull_major, "scull");    

    if (scull_devices) {
        for (i=0; i<scull_nr_devs; i++) {
            scull_trim(scull_devices+i);
        }
        kfree(scull_devices);
    }
}


int scull_init_module(void)
{
    int result, i;  

    /*
     * Register your major, and accept a dynamic number. This is the
     * first thing to do, in order to avoid releasing other module's
     * fops in scull_cleanup_module()
     */
     //!! multiple devices use the same driver as long as their major number are identical
    result = register_chrdev(0, "scull", &scull_fops);
    if (result < 0) {
        printk(KERN_WARNING "scull: can't get major\n");
        return result;
    }
    if (scull_major == 0) scull_major = result; /* dynamic */

    /* 
     * allocate the devices -- we can't have them static, as the number
     * can be specified at load time
     */
    scull_devices = kmalloc(scull_nr_devs * sizeof(Scull_Dev), GFP_KERNEL);
    if (!scull_devices) {
        result = -ENOMEM;
        goto fail;
    }
    memset(scull_devices, 0, scull_nr_devs * sizeof(Scull_Dev));
    for (i=0; i < scull_nr_devs; i++) {
        scull_devices[i].quantum = scull_quantum;
        scull_devices[i].qset = scull_qset;
        sema_init(&scull_devices[i].sem, 1);
    }

    return 0; /* succeed */

fail:
    scull_cleanup_module();
    return result;
}



module_init(scull_init_module);
module_exit(scull_cleanup_module);





















