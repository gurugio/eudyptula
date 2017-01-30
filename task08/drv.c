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

MODULE_LICENSE("GPL");

#define MYID "832a404c8ce0"
#define MYIDLEN 12

static struct dentry *eudyptula_debugfs_root;


static ssize_t eudyptula_id_read(struct file *file, char __user *buf,
			   size_t count, loff_t *ppos)
{
	return simple_read_from_buffer(buf, count, ppos, MYID, 12);
}

static ssize_t eudyptula_id_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
	char kbuf[20] = {0,};
	ssize_t ret;

	ret = simple_write_to_buffer(kbuf, 19, ppos, buf, count);
	if (ret < 0)
		return ret;
	if (ret < MYIDLEN || strncmp(kbuf, MYID, ret))
		return -EINVAL;
	return count;
}

static const struct file_operations eudyptula_id_fops = {
	.owner = THIS_MODULE,
	.read = eudyptula_id_read,
	.write = eudyptula_id_write,
};

static int __init hello_init(void)
{
	struct dentry *debugfs_id;

	eudyptula_debugfs_root = debugfs_create_dir("eudyptula", NULL);
	if (!eudyptula_debugfs_root
	    || eudyptula_debugfs_root == ERR_PTR(-ENODEV)) {
		pr_err("failed to create debugfs dir: eudyptula\n");
		return 0;
	}

	debugfs_id = debugfs_create_file_size("id",
					      S_IRUSR | S_IWUSR |
					      S_IRGRP | S_IWGRP |
					      S_IROTH | S_IWOTH,
					      eudyptula_debugfs_root,
					      NULL,
					      &eudyptula_id_fops,
					      strlen(MYID));
	if (!debugfs_id
	    || debugfs_id == ERR_PTR(-ENODEV)) {
		pr_err("failed to create debugfs file: id\n");
		return 0;
	}
	return 0;
}

static void __exit hello_exit(void)
{
	debugfs_remove_recursive(eudyptula_debugfs_root);
}

module_init(hello_init);
module_exit(hello_exit);
