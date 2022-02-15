#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");

static struct usb_device_id usb_ids[] = {
	{.driver_info = 42},
	{}
};

MODULE_DEVICE_TABLE(usb, usb_ids);

static int usbProbe(struct usb_interface *interface, struct usb_device_id const *id)
{
	printk(KERN_INFO "Vendor: %x, Product: %x \n", id->idVendor, id->idProduct);
	return 0;
}

static void usbDisconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "Device disconneted\n");
}

static struct usb_driver skull_driver = 
{
	.name = "UsbDriverTestModule",
	.id_table = usb_ids,
	.probe = usbProbe,
	.disconnect = usbDisconnect,
};

static int __init init_driver(void)
{
	printk(KERN_INFO "USB driver has been loaded\n");
	return usb_register(&skull_driver);
}

static void __exit close_driver(void)
{
	usb_deregister(&skull_driver);
	printk(KERN_INFO "USB driver has been closed\n");
}

module_init(init_driver);
module_exit(close_driver);


