/**
 * @\file	k_timer_module.c
 * @\author	Sanju Prakash Kannioth
 * @\brief	This program calls a timer system call every few milliseconds specified by the user
 * @\date	02/17/2019
 * References: https://www.tldp.org/LDP/lkmpg/2.6/html/x323.html
 *
 */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

struct timer_list timer;

static int delay = 500; // In millisecond
static unsigned long timer_count = 0; // Count the number of times the counter triggered
static char *name = "DEFAULT"; // Default value if command line value isn't passed


module_param(name, charp, 0000);
MODULE_PARM_DESC(name, "A character string");
module_param(delay, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(delay, "An integer");

void timer_handler(unsigned long dummy)
{
	timer_count++;
	printk("Name = %s",name);
	printk(KERN_INFO "Timer triggered\n Timer count = %ld \n Period in millisecond = %d", timer_count, delay);
	mod_timer(&timer, jiffies + (delay/10));
}

static int timer_init(void)
{
	
	printk(KERN_INFO "Initialized timer module\n");
	timer.expires = jiffies + (delay/10);
	timer.function = &timer_handler;
	timer.data = 0;
	init_timer(&timer); // Initialize timer

	add_timer(&timer); 
	return 0;
}

static void timer_exit(void)
{
	printk(KERN_INFO "Deleting timer module\n");
	del_timer(&timer);
}

module_init(timer_init);
module_exit(timer_exit);
	
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sanju Prakash Kannioth");
MODULE_DESCRIPTION("Kernel Timer Module");
MODULE_VERSION("1.0");