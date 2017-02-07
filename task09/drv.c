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
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/sysfs.h>


MODULE_LICENSE("GPL");

#define MYID "832a404c8ce0"
#define MYIDLEN 12

char foo_buf[4096];
size_t foo_pos;
spinlock_t foo_lock;

struct kobject *my_kobj;

static ssize_t foo_show(struct kobject *kobj,
			struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%s", foo_buf);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	if (count > 4095)
		count = 4095;
	strncpy(foo_buf, buf, count);
	foo_buf[count] = 0;
	foo_pos = count;
	return count;
}

static struct kobj_attribute foo_attr =
	__ATTR_RW(foo);

static ssize_t jiffies_show(struct kobject *kobj,
			    struct kobj_attribute *attr,
			    char *buf)
{
	return sprintf(buf, "%llu\n", jiffies_64);
}

static struct kobj_attribute jiffies_attr =
	__ATTR_RO(jiffies);

static ssize_t id_show(struct kobject *kobj,
		       struct kobj_attribute *attr,
		       char *buf)
{
	return sprintf(buf, "%s\n", MYID);
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	if (count < MYIDLEN || strncmp(buf, MYID, count))
		return -EINVAL;
	return count;
}

static struct kobj_attribute id_attr =
	__ATTR_RW(id);

static struct attribute *attrs[] = {
	&id_attr.attr,
	&jiffies_attr.attr,
	&foo_attr.attr,
	NULL
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static int __init hello_init(void)
{
	int retval;

	my_kobj = kobject_create_and_add("eudyptula", kernel_kobj);
	if (!my_kobj)
		return -ENOMEM;

	spin_lock_init(&foo_lock);

	retval = sysfs_create_group(my_kobj, &attr_group);
	if (retval)
		kobject_put(my_kobj);

	return retval;
}

static void __exit hello_exit(void)
{
	kobject_put(my_kobj);
}

module_init(hello_init);
module_exit(hello_exit);
