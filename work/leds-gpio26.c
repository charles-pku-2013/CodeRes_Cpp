/*
 * LEDs driver for GPIOs
 *
 * Copyright (C) 2007 8D Technologies inc.
 * Raphael Assenat <raph@8d.com>
 * Copyright (C) 2008 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/kmod.h>
	 
#include <asm/gpio.h>
	 
	 //
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>
#include <linux/sched.h>

#define GPIO_BTN 45
#define GPIO_RST 46

#define INTERNAL_REG_BASE_ADDR  0xf1000000

#define MV_REG_READ(offset) my_le32_to_cpu(* (volatile unsigned int *) (INTERNAL_REG_BASE_ADDR + offset))
#define MV_REG_WRITE(offset,data) *(volatile unsigned int *) (INTERNAL_REG_BASE_ADDR + offset) = my_cpu_to_le32 (data)
#define my_cpu_to_le32(x) my_le32_to_cpu((x))

#define PROC_ENTRY_NAME "gcamd.pid"
#define BUFLEN 10

MODULE_LICENSE("GPL");

static char *envp[] = { "HOME=/",		// for run user cmd
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

static struct work_struct short_wq;

static struct proc_dir_entry *my_proc_entry = 0;
static int gcampid = -1;
static unsigned long time_old = 0;
static int reset = 0;

static ssize_t entry_read(char *buffer,
	      char **buffer_location,
	      off_t offset, int count, int *eof, void *data)
{	
	ssize_t len;
	sprintf( buffer, "%d", gcampid );
	len = strlen(buffer);
	*eof = 1;
	return len;
}

static ssize_t entry_write(struct file *file, const char *buffer, unsigned long count,
		   void *data)
{	
//	printk( "entry_write() count=%d\n", count );
	char buf[BUFLEN+1];

	if( count > BUFLEN )
		count = BUFLEN;

	/* write data to the buffer */
	if ( copy_from_user(buf, buffer, count) ) {
		return -EFAULT;
	}
	
	buf[count] = 0;
	sscanf(buf, "%d", &gcampid);
	
	printk( "Updating gcamd.pid = %d\n", gcampid );
	
	return count;
}


unsigned long my_le32_to_cpu (unsigned long x)
{
        return (((x & 0x000000ffU) << 24) |
                ((x & 0x0000ff00U) << 8) |
                ((x & 0x00ff0000U) >> 8) | ((x & 0xff000000U) >> 24));
}

static irqreturn_t gpio_btn_handler(int irq, void *dev_id);
static void __init gpio_btn(void)
{
	int error;  int irq;
	orion_gpio_set_valid(GPIO_BTN, GPIO_INPUT_OK);

	error = gpio_request(GPIO_BTN, "gpio_btn");
	if ( error<0 ) { pr_err("gpio_btn: failed to request GPIO %d, error %d \n",GPIO_BTN, error); }

	error = gpio_direction_input(GPIO_BTN);
	if ( error<0 ) 
	{ pr_err("gpio_btn: failed to configure input for GPIO %d, error %d \n",GPIO_BTN, error); }

	irq = gpio_to_irq(GPIO_BTN);
	if ( irq < 0) 
	{ pr_err("gpio_btn: Unable to get irq number for GPIO %d, error %d \n",GPIO_BTN, irq); }

//        error = request_irq(irq, gpio_btn_handler, IRQF_TRIGGER_RISING, "gpio_btn", NULL);
        error = request_irq(irq, gpio_btn_handler, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "gpio_btn", NULL);
	if ( error ) 
	{ pr_err("gpio_btn: failed to request_irq for GPIO/irq %d %d, error %d \n",GPIO_BTN,irq, error); }

	printk("gpio_btn is called sucessfully! \n");

	orion_gpio_set_valid(GPIO_RST,GPIO_OUTPUT_OK);
//	error = gpio_direction_output(GPIO_RST);
}

static void call_user_cmd( const char *arg )
{
	char *argv[2];

	argv[1] = (char*)NULL;
	argv[0] = kmalloc( 20, GFP_KERNEL );
	strncpy( argv[0], arg, 20 );

	if( call_usermodehelper( argv[0], argv, envp, 0 ) )
		printk( "cannot run user space cmd: %s!\n", arg );	

	kfree( argv[0] );
}

static void notify_gcam( void *arg )
{
	struct task_struct *process = NULL;
	int ret;
	int signo;

	signo = reset ? SIGUSR2 : SIGUSR1;
	reset = 0;
	
	printk("sending signal %d to gcam process %d\n", signo, gcampid);	

	if( gcampid < 0 ) {
		printk( "gcamd has not started!\n" );
		return;
	}
	
	if( 0 == gcampid ) {
		call_user_cmd( "/sbin/start_ap.sh" );
		return;
	}
	
	rcu_read_lock();
	process = find_task_by_vpid(gcampid);  //find the task_struct associated with this pid
	if(process == NULL){
		printk("no such pid\n");
		rcu_read_unlock();
		return;
	}
	rcu_read_unlock();
	
	ret = send_sig_info(SIGUSR2, 1, process);   //send the signal
	if (ret < 0) {
		printk("error sending signal\n");
		return;
	}
}

static irqreturn_t gpio_btn_handler(int irq, void *dev_id)
{
	unsigned long time_now;
	long time_diff;

	printk("  the gpio is triggered, the irq is %d !\n",irq);
	
	time_now = jiffies;
	
	if( !time_old )
		time_old = time_now;
	else {
		time_diff = (long)time_now - (long)time_old;
		time_old = 0;
		if( (time_diff / HZ) > 5 )
			reset = 1;
		schedule_work(&short_wq);
	}
	
/*	value1 = MV_REG_READ(0x10000);
	value1 &=0xfffffff ;
	MV_REG_WRITE(0x10000,value1);
	
	value1 = MV_REG_READ(0x10104);
        value1 &=~ (1 << 7 );
        MV_REG_WRITE(0x10104,value1);

	value1 = MV_REG_READ(0x10100);
        value1 &=~ (1 << 7 );
        MV_REG_WRITE(0x10100,value1);
*/
	return 1;
}





struct gpio_led_data {
	struct led_classdev cdev;
	unsigned gpio;
	struct work_struct work;
	u8 new_level;
	u8 can_sleep;
	u8 active_low;
	int (*platform_gpio_blink_set)(unsigned gpio,
			unsigned long *delay_on, unsigned long *delay_off);
};

static void gpio_led_work(struct work_struct *work)
{
	struct gpio_led_data	*led_dat =
		container_of(work, struct gpio_led_data, work);

	gpio_set_value_cansleep(led_dat->gpio, led_dat->new_level);
}

static void gpio_led_set(struct led_classdev *led_cdev,
	enum led_brightness value)
{
	struct gpio_led_data *led_dat =
		container_of(led_cdev, struct gpio_led_data, cdev);
	int level;

	if (value == LED_OFF)
		level = 0;
	else
		level = 1;

	if (led_dat->active_low)
		level = !level;

	/* Setting GPIOs with I2C/etc requires a task context, and we don't
	 * seem to have a reliable way to know if we're already in one; so
	 * let's just assume the worst.
	 */
	if (led_dat->can_sleep) {
		led_dat->new_level = level;
		schedule_work(&led_dat->work);
	} else
		gpio_set_value(led_dat->gpio, level);
}

static int gpio_blink_set(struct led_classdev *led_cdev,
	unsigned long *delay_on, unsigned long *delay_off)
{
	struct gpio_led_data *led_dat =
		container_of(led_cdev, struct gpio_led_data, cdev);

	return led_dat->platform_gpio_blink_set(led_dat->gpio, delay_on, delay_off);
}

static int __devinit create_gpio_led(const struct gpio_led *template,
	struct gpio_led_data *led_dat, struct device *parent,
	int (*blink_set)(unsigned, unsigned long *, unsigned long *))
{
	int ret, state;

	led_dat->gpio = -1;

	/* skip leds that aren't available */
	if (!gpio_is_valid(template->gpio)) {
		printk(KERN_INFO "Skipping unavailable LED gpio %d (%s)\n",
				template->gpio, template->name);
		return 0;
	}

	ret = gpio_request(template->gpio, template->name);
	if (ret < 0)
		return ret;

	led_dat->cdev.name = template->name;
	led_dat->cdev.default_trigger = template->default_trigger;
	led_dat->gpio = template->gpio;
	led_dat->can_sleep = gpio_cansleep(template->gpio);
	led_dat->active_low = template->active_low;
	if (blink_set) {
		led_dat->platform_gpio_blink_set = blink_set;
		led_dat->cdev.blink_set = gpio_blink_set;
	}
	led_dat->cdev.brightness_set = gpio_led_set;
	if (template->default_state == LEDS_GPIO_DEFSTATE_KEEP)
		state = !!gpio_get_value(led_dat->gpio) ^ led_dat->active_low;
	else
		state = (template->default_state == LEDS_GPIO_DEFSTATE_ON);
	led_dat->cdev.brightness = state ? LED_FULL : LED_OFF;
	if (!template->retain_state_suspended)
		led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;

	ret = gpio_direction_output(led_dat->gpio, led_dat->active_low ^ state);
	if (ret < 0)
		goto err;

	INIT_WORK(&led_dat->work, gpio_led_work);

	ret = led_classdev_register(parent, &led_dat->cdev);
	if (ret < 0)
		goto err;

	return 0;
err:
	gpio_free(led_dat->gpio);
	return ret;
}

static void delete_gpio_led(struct gpio_led_data *led)
{
	if (!gpio_is_valid(led->gpio))
		return;
	led_classdev_unregister(&led->cdev);
	cancel_work_sync(&led->work);
	gpio_free(led->gpio);
}

#ifdef CONFIG_LEDS_GPIO_PLATFORM
static int __devinit gpio_led_probe(struct platform_device *pdev)
{
	struct gpio_led_platform_data *pdata = pdev->dev.platform_data;
	struct gpio_led_data *leds_data;
	int i, ret = 0;

	if (!pdata)
		return -EBUSY;

	leds_data = kzalloc(sizeof(struct gpio_led_data) * pdata->num_leds,
				GFP_KERNEL);
	if (!leds_data)
		return -ENOMEM;

	for (i = 0; i < pdata->num_leds; i++) {
		ret = create_gpio_led(&pdata->leds[i], &leds_data[i],
				      &pdev->dev, pdata->gpio_blink_set);
		if (ret < 0)
			goto err;
	}

	platform_set_drvdata(pdev, leds_data);

	return 0;

err:
	for (i = i - 1; i >= 0; i--)
		delete_gpio_led(&leds_data[i]);

	kfree(leds_data);

	return ret;
}

static int __devexit gpio_led_remove(struct platform_device *pdev)
{
	int i;
	struct gpio_led_platform_data *pdata = pdev->dev.platform_data;
	struct gpio_led_data *leds_data;

	leds_data = platform_get_drvdata(pdev);

	for (i = 0; i < pdata->num_leds; i++)
		delete_gpio_led(&leds_data[i]);

	kfree(leds_data);

	return 0;
}

static struct platform_driver gpio_led_driver = {
	.probe		= gpio_led_probe,
	.remove		= __devexit_p(gpio_led_remove),
	.driver		= {
		.name	= "leds-gpio",
		.owner	= THIS_MODULE,
	},
};

MODULE_ALIAS("platform:leds-gpio");
#endif /* CONFIG_LEDS_GPIO_PLATFORM */

/* Code to create from OpenFirmware platform devices */
#ifdef CONFIG_LEDS_GPIO_OF
#include <linux/of_platform.h>
#include <linux/of_gpio.h>

struct gpio_led_of_platform_data {
	int num_leds;
	struct gpio_led_data led_data[];
};

static int __devinit of_gpio_leds_probe(struct of_device *ofdev,
					const struct of_device_id *match)
{
	struct device_node *np = ofdev->node, *child;
	struct gpio_led_of_platform_data *pdata;
	int count = 0, ret;

	/* count LEDs defined by this device, so we know how much to allocate */
	for_each_child_of_node(np, child)
		count++;
	if (!count)
		return 0; /* or ENODEV? */

	pdata = kzalloc(sizeof(*pdata) + sizeof(struct gpio_led_data) * count,
			GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	for_each_child_of_node(np, child) {
		struct gpio_led led = {};
		enum of_gpio_flags flags;
		const char *state;

		led.gpio = of_get_gpio_flags(child, 0, &flags);
		led.active_low = flags & OF_GPIO_ACTIVE_LOW;
		led.name = of_get_property(child, "label", NULL) ? : child->name;
		led.default_trigger =
			of_get_property(child, "linux,default-trigger", NULL);
		state = of_get_property(child, "default-state", NULL);
		if (state) {
			if (!strcmp(state, "keep"))
				led.default_state = LEDS_GPIO_DEFSTATE_KEEP;
			else if(!strcmp(state, "on"))
				led.default_state = LEDS_GPIO_DEFSTATE_ON;
			else
				led.default_state = LEDS_GPIO_DEFSTATE_OFF;
		}

		ret = create_gpio_led(&led, &pdata->led_data[pdata->num_leds++],
				      &ofdev->dev, NULL);
		if (ret < 0) {
			of_node_put(child);
			goto err;
		}
	}

	dev_set_drvdata(&ofdev->dev, pdata);

	return 0;

err:
	for (count = pdata->num_leds - 2; count >= 0; count--)
		delete_gpio_led(&pdata->led_data[count]);

	kfree(pdata);

	return ret;
}

static int __devexit of_gpio_leds_remove(struct of_device *ofdev)
{
	struct gpio_led_of_platform_data *pdata = dev_get_drvdata(&ofdev->dev);
	int i;

	for (i = 0; i < pdata->num_leds; i++)
		delete_gpio_led(&pdata->led_data[i]);

	kfree(pdata);

	dev_set_drvdata(&ofdev->dev, NULL);

	return 0;
}

static const struct of_device_id of_gpio_leds_match[] = {
	{ .compatible = "gpio-leds", },
	{},
};

static struct of_platform_driver of_gpio_leds_driver = {
	.driver = {
		.name = "of_gpio_leds",
		.owner = THIS_MODULE,
	},
	.match_table = of_gpio_leds_match,
	.probe = of_gpio_leds_probe,
	.remove = __devexit_p(of_gpio_leds_remove),
};
#endif

static int __init gpio_led_init(void)
{
	int ret;

#ifdef CONFIG_LEDS_GPIO_PLATFORM	
	ret = platform_driver_register(&gpio_led_driver);
	if (ret)
		return ret;
#endif
#ifdef CONFIG_LEDS_GPIO_OF
	ret = of_register_platform_driver(&of_gpio_leds_driver);
#endif
#ifdef CONFIG_LEDS_GPIO_PLATFORM	
	if (ret)
		platform_driver_unregister(&gpio_led_driver);
#endif

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

	INIT_WORK(&short_wq, notify_gcam);

	gpio_btn();	//by steven adding

	return ret;
}

static void __exit gpio_led_exit(void)
{
#ifdef CONFIG_LEDS_GPIO_PLATFORM
	platform_driver_unregister(&gpio_led_driver);
#endif
#ifdef CONFIG_LEDS_GPIO_OF
	of_unregister_platform_driver(&of_gpio_leds_driver);
#endif
	remove_proc_entry( PROC_ENTRY_NAME, NULL );
	flush_scheduled_work();
}

module_init(gpio_led_init);
module_exit(gpio_led_exit);

//MODULE_AUTHOR("Raphael Assenat <raph@8d.com>, Trent Piepho <tpiepho@freescale.com>");
//MODULE_DESCRIPTION("GPIO LED driver");
//MODULE_LICENSE("GPL");
