#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static struct _device
{
	size_t openCount, totalDataWritten;
	bool isReaded;
	struct cdev c_dev;
	dev_t deviceNumber;
};


static struct _device *myDevice;

static int devOpen(struct inode * n, struct file * f)
{
	myDevice->openCount++;			
	myDevice->isReaded = false;
	return 0;
}

static ssize_t devWrite(struct file * n, char const __user * c, size_t size, loff_t *l)
{
	myDevice->totalDataWritten += size;
	return size;
}

static ssize_t devRead(struct file *fp, char __user *buf, size_t size, loff_t *off)
{
	if (myDevice->isReaded)
	{
		return 0;
	}

	char *tmpbuf = (char*) kmalloc(size, GFP_KERNEL);

	//unsafe 
	sprintf(tmpbuf,"%zu %zu\n\0", myDevice->openCount, myDevice->totalDataWritten);

	ssize_t bytesCopied =  strlen(tmpbuf) - copy_to_user(buf, tmpbuf, strlen(tmpbuf));
	*off = bytesCopied;
	myDevice->isReaded = true;
	kfree(tmpbuf);
	return bytesCopied;
}

static struct file_operations devOPS = 
{
	.owner = THIS_MODULE,
	.open = devOpen,
	.write = devWrite,
	.read = devRead,  		
}; 

static const int major = 240, minor = 0;
static dev_t devID;
static const char *name = "solution_node";

static int __init deviceInit(void)
{
	myDevice = (struct _device *) kmalloc(sizeof(struct _device), GFP_KERNEL);
	devID = MKDEV(major, minor);

	if (register_chrdev_region(devID, 256, name) != 0)
	{
		return -1;
	}

	cdev_init(&myDevice->c_dev, &devOPS);
	myDevice->c_dev.owner = THIS_MODULE;
	myDevice->c_dev.ops = &devOPS;
	myDevice->openCount = 0;
	myDevice->totalDataWritten = 0;
	myDevice->deviceNumber = MKDEV(major, minor);

	if (cdev_add(&myDevice->c_dev, myDevice->deviceNumber, 256))
	{
		return -2;
	}	
		
	return 0;
}

static void __exit deviceExit(void)
{
	cdev_del(&myDevice->c_dev);
	unregister_chrdev_region(devID, 256);
	return;
}

module_init(deviceInit);
module_exit(deviceExit);
