#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

static struct _device
{
	size_t openCount, totalDataWritten;
	struct cdev c_dev;
	dev_t deviceNumber;
};


static struct _device *myDevice;

static int devOpen(struct inode * n, struct file * f)
{
	myDevice->openCount++;			
	return 0;
}

static ssize_t devWrite(struct file * n, char const __user * c, size_t size, loff_t *l)
{
	myDevice->totalDataWritten += size;
	return size;
}

static ssize_t devRead(struct file *fp, char __user *buf, size_t size, loff_t *off)
{
	return size;
}

static struct file_operations devOPS = 
{
	.owner = THIS_MODULE,
	.open = devOpen,
	.write = devWrite,
	.read = devRead,  		
}; 

static const int major = 255, minor = 0;
static dev_t devID;
static const char *name = "sol";

static int __init deviceInit(void)
{
	myDevice = (struct _device *) kmalloc(sizeof(struct _device), GFP_KERNEL);
	devID = MKDEV(major, minor);

	printk(KERN_INFO "Major %i, minor %i\n", MAJOR(devID), MINOR(devID));

	if (register_chrdev_region(devID, 1, name) != 0)
	{
		printk(KERN_ERR "Cannot register char device region\n");
		return 1;
	}
	
	printk(KERN_INFO "HERE\n");

	cdev_init(&myDevice->c_dev, &devOPS);
	myDevice->c_dev.owner = THIS_MODULE;
	myDevice->c_dev.ops = &devOPS;
	myDevice->openCount = 0;
	myDevice->totalDataWritten = 0;
	myDevice->deviceNumber = MKDEV(major, minor + 1);

	printk(KERN_INFO "After initing\n");

	if (cdev_add(&myDevice->c_dev, myDevice->deviceNumber, 1))
	{
		printk(KERN_ERR "Failed to Add device number\n");
		return 2;
	}	
	
	printk(KERN_INFO "Before exit\n");	
	return 0;
}

static void __exit deviceExit(void)
{
	cdev_del(&myDevice->c_dev);
	printk(KERN_INFO "Device's major: %i, minor: %i\n", MAJOR(myDevice->deviceNumber), MINOR(myDevice->deviceNumber)); 
	unregister_chrdev_region(MKDEV(major, minor), 1);
	return;
}

module_init(deviceInit);
module_exit(deviceExit);
