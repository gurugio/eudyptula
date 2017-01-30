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
#include <linux/usb/input.h>
#include <linux/hid.h>


MODULE_LICENSE("GPL");

static struct usb_device_id usb_task05_id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			     USB_INTERFACE_SUBCLASS_BOOT,
			     USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{ }
};

MODULE_DEVICE_TABLE(usb, usb_task05_id_table);

static int __init hello_init(void)
{
	pr_debug("Hello USB-driver\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_debug("Goodbye USB-driver\n");
}

module_init(hello_init);
module_exit(hello_exit);
