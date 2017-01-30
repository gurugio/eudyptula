#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>

#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");


static int __init hello_init(void)
{
	pr_debug("Hello World!\n");
	return 0;
}

static void __exit hello_exit(void)
{
}

module_init(hello_init);
module_exit(hello_exit);
