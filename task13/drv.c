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
#include <linux/stddef.h>

MODULE_LICENSE("GPL");

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head list;
};

static LIST_HEAD(all_identity);
static struct kmem_cache *id_cache;


static int identity_create(char *name, int id)
{
	struct identity *idp;

	if (!name || id <= 0)
		return -EINVAL;

	idp = kmem_cache_alloc(id_cache, GFP_KERNEL);
	if (!id)
		return -ENOMEM;

	strncpy(idp->name, name, 19);
	idp->id = id;
	idp->busy = false;
	list_add(&idp->list, &all_identity);
	return 0;
}

static struct identity *identity_find(int id)
{
	struct identity *idp;

	list_for_each_entry(idp, &all_identity, list) {
		if (idp->id == id)
			return idp;
	}
	return NULL;
}

static void identity_destroy(int id)
{
	struct identity *idp, *idn;

	list_for_each_entry_safe(idp, idn, &all_identity, list) {
		if (idp->id == id) {
			list_del(&idp->list);
			kmem_cache_free(id_cache, idp);
		}
	}
}

static void identity_flush(void)
{
	struct identity *idp, *idn;

	list_for_each_entry_safe(idp, idn, &all_identity, list) {
		list_del(&idp->list);
		kfree(idp);
	}
}

static int __init hello_init(void)
{
	struct identity *temp;
	int ret;

	pr_debug("start hello_init\n");

	id_cache = KMEM_CACHE(identity, 0);
	if (!id_cache) {
		pr_err("KMEM_CACHE failed\n");
		return 0;
	}

	ret = identity_create("Alice", 1);
	if (ret)
		pr_err("fail to add Alice\n");
	else
		pr_debug("add Alice\n");

	ret = identity_create("Bob", 2);
	if (ret)
		pr_err("fail to add Bob\n");
	else
		pr_debug("add Bob\n");

	ret = identity_create("Dave", 3);
	if (ret)
		pr_err("fail to add Dave\n");
	else
		pr_debug("add Dave\n");

	ret = identity_create("Gena", 10);
	if (ret)
		pr_err("fail to add Gena\n");
	else
		pr_debug("add Gena\n");

	temp = identity_find(3);
	if (IS_ERR_OR_NULL(temp))
		pr_debug("id 3 not found\n");
	pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return 0;
}

static void __exit hello_exit(void)
{
	if (!list_empty(&all_identity)) {
		pr_err("There are remain identities\n");
		identity_flush();
	}
	kmem_cache_destroy(id_cache);
}

module_init(hello_init);
module_exit(hello_exit);
