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
#include <linux/miscdevice.h>
MODULE_LICENSE("GPL");

char myid[] = "832a404c8ce0";
#define MYIDLEN 12

static ssize_t task06_read(struct file *file, char __user *buf,
			   size_t count, loff_t *ppos)
{
	return simple_read_from_buffer(buf, count, ppos, myid, 12);
}

static ssize_t task06_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
	char kbuf[20] = {0,};
	ssize_t ret;

	ret = simple_write_to_buffer(kbuf, 19, ppos, buf, count);
	if (ret < 0)
		return ret;
	if (ret < MYIDLEN || strncmp(kbuf, myid, ret))
		return -EINVAL;
	return count;
}

static const struct file_operations task06_fops = {
	.owner = THIS_MODULE,
	.read = task06_read,
	.write = task06_write,
};

static struct miscdevice task06_device = {
	MISC_DYNAMIC_MINOR,
	"eudyptula",
	&task06_fops
};

static int __init hello_init(void)
{
	int ret;

	pr_debug("start task06\n");
	ret = misc_register(&task06_device);
	if (ret) {
		pr_err("task06: failed to misc_register\n");
		return ret;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	misc_deregister(&task06_device);
	pr_debug("end task06\n");
}

module_init(hello_init);
module_exit(hello_exit);
