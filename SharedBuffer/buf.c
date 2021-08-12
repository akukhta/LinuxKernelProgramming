#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/rwsem.h>
#include "LinkedBuffer.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexey Kukhta");

struct Shared_Buffer
{	
	size_t totalSize;
	size_t deviceID;
	struct BufferList *head;
	struct cdev dev;
	struct rwsemaphore *sem;
};

static dev_t majorNumber;
static int deviceCount = 1;
static int minor = 0;
static struct class *deviceClass;
static struct device *devices;
static struct Shared_Buffer *buffers;

module_param(deviceCount, int, S_IRUGO | S_IWUSR);


static int sharedBufferOpen(struct inode *nod, struct file *fp)
{
	struct Shared_Buffer *buf = container_of(nod->i_cdev, struct Shared_Buffer, dev);
	fp->private_data = buf;
	printk(KERN_INFO "Device has been opened\n");
	return 0;
}

static ssize_t sharedBufferRead(struct file *fp, char __user *buf, size_t size, loff_t *off)
{
	printk(KERN_INFO "Reading from Shared Buffer %i\n", ((struct Shared_Buffer*)fp->private_data)->deviceID);
	return 1;
} 

static ssize_t sharedBufferWrite(struct file *fp, char const __user *buffer, size_t size, loff_t *off)
{
	printk(KERN_INFO "Writing to Shared Buffer\n");
	return 1;
}

static int sharedBufferRelease(struct inode *nod, struct file *fp)
{
	printk(KERN_INFO "Device has been released\n");
	return 0;
}


static struct file_operations SharedBufferFOPS = 
{
	.owner = THIS_MODULE,
	.open = sharedBufferOpen,
	.read = sharedBufferRead,
	.write = sharedBufferWrite,
	.release = sharedBufferRelease, 	
};

static int __init SharedBufferInit(void)
{
	if (alloc_chrdev_region(&majorNumber, minor, deviceCount, "SharedBuffer"))
	{
		printk(KERN_ERR "Failed to allocate minor numbers region\n");
		return 1;
	}
	
	deviceClass = class_create(THIS_MODULE, "SharedBuffer");
	
	if (deviceClass == NULL)
	{
		unregister_chrdev_region(majorNumber, deviceCount);
		printk(KERN_ERR "Failed to create class in sysfs\n");
		return 2;
	}
	
	
	if (deviceCount == 1)
	{
		devices = device_create(deviceClass, NULL, majorNumber, NULL, "SharedBuffer");
	}
	
	else
	{
		devices = (struct device*) kmalloc(sizeof(struct device) * deviceCount, GFP_USER);
		
		int i = 0;
		for (; i < deviceCount; i++)
		{
			dev_t number = MKDEV(MAJOR(majorNumber), i);
			devices[i] = *(device_create(deviceClass, NULL, number, NULL, "SharedBuffer%i", i + 1));
		}
	}
	
	if (devices == NULL)
	{
		class_destroy(deviceClass);
		unregister_chrdev_region(majorNumber, deviceCount);
		printk(KERN_ERR "Unable to register device\n");
		return 3;
	}
	
	buffers = (struct Shared_Buffer*) kmalloc(sizeof(struct Shared_Buffer) * deviceCount, GFP_USER);	
	
	if (buffers == NULL)
	{
		printk(KERN_ERR "Failed to allocate memory\n");
	}
	
		size_t i = 0;
		for (; i < deviceCount; i++)
		{
			cdev_init(&buffers[i].dev, &SharedBufferFOPS);
			buffers[i].dev.owner = THIS_MODULE;
			buffers[i].dev.ops = &SharedBufferFOPS;
			buffers[i].deviceID = i + 1;
			dev_t number = MKDEV(MAJOR(majorNumber), i);
			
			if (cdev_add(&buffers[i].dev, number, 1))
			{
				printk(KERN_ERR "Can`t add cdev\n");
				return 4;
			}		
		}
	
	printk(KERN_INFO "Shared buffer\\s has\\ve been inited\n");
	return 0;
}

static void __exit SharedBufferExit(void)
{
	size_t i = 0;
	
	for (; i < deviceCount; i++)
	{
		cdev_del(&buffers[i].dev);
	}
	
	printk(KERN_INFO "Here1\n");
	
	if (deviceClass != NULL)
	{
		if (deviceCount == 1)
		{
			device_destroy(deviceClass, majorNumber);
			printk(KERN_INFO "Here2\n");
		}
		else
		{
			i = 0;
			for (; i < deviceCount; i++)
			{
				dev_t number = MKDEV(MAJOR(majorNumber), i);
				device_destroy(deviceClass, number);
			}
			
			printk(KERN_INFO "Here2\n");
		}
		
		class_destroy(deviceClass);
		printk(KERN_INFO "Here3\n");
	}
	
	unregister_chrdev_region(majorNumber, deviceCount);
	printk(KERN_INFO "Here4\n");
	kfree(buffers);
	return;
}

module_init(SharedBufferInit);
module_exit(SharedBufferExit);






