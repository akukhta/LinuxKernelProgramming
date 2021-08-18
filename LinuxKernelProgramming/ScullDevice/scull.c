#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

MODULE_LICENSE("GPL");

static int minor = 0;
static const char className[] = "ScullDevice";
static dev_t classMajor;
static int const deviceCount = 4;
static struct class *devClass;
static struct device *devDevice;
static struct cdev *myCdev;

static int scullOpen(struct inode *node, struct file *filep)
{
	printk(KERN_INFO "Scull has been opened\n");
	return 0;
}

static int scullRelease(struct inode *node, struct file *filep)
{
	printk(KERN_INFO "Scull has been released\n");
	return 0;
}

static ssize_t scullRead(struct file *filep, char __user *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_INFO "Scull's read operation\n");
	return 0;
}

static ssize_t scullWrite(struct file *filep, char const __user *buf, size_t count, loff_t *fpos)
{
	printk(KERN_INFO "Scull's write operation\n");
	return 0;
}

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	//.open = scullOpen,
	//.release = scullRelease,
	.read = scullRead,
	//.write = scullWrite,
};

static int __init scullInit(void)
{
	if (alloc_chrdev_region(&classMajor, minor, deviceCount, className))
	{
		printk(KERN_ALERT "Can`t get major number for device!\n");
		return 1;
	}
	else
	{
		char const className1[] = "SCULLDEVIL";
		devClass = class_create(THIS_MODULE, "SCULLDEVIL");
		printk(KERN_INFO "Major number: %i\n", (int) MAJOR(classMajor));
		
		if (devClass == NULL)
		{
			printk(KERN_ALERT "Can`t create class\n");
			unregister_chrdev_region(classMajor, deviceCount);
			return 1;
		}

		devDevice = device_create(devClass, NULL, classMajor, NULL, "SCULL");
		printk(KERN_INFO "HERE2222\n");
		if (devDevice = NULL)
		{
			printk(KERN_ALERT "Can`t create device\n");
			class_destroy(devClass);
			unregister_chrdev_region(classMajor, deviceCount);
			return 1;
		}
	}
	myCdev = cdev_alloc();
	cdev_init(myCdev, &fops);
	cdev_add(myCdev, classMajor, deviceCount);
	return 0;
}

static void __exit scullCleanup(void)
{
	cdev_del(myCdev);
	printk(KERN_ALERT "HERE1\n");
	
	if (devClass == NULL)
		printk(KERN_ALERT "NULLL\n");
	
	printk(KERN_INFO "%i , %i", MAJOR(classMajor), MINOR(classMajor));
	device_destroy(devClass, classMajor);
	printk(KERN_ALERT "HERE2\n");
	class_destroy(devClass);
	printk(KERN_ALERT "HETR3\n");
	unregister_chrdev_region(classMajor, deviceCount);
	printk(KERN_INFO "Scull device has been unloaded\n");
}

module_init(scullInit);
module_exit(scullCleanup);

