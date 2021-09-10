#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/rwsem.h>
#include <linux/ioctl.h>
#include "LinkedBuffer.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexey Kukhta");

struct Shared_Buffer
{	
	size_t totalSize;
	size_t deviceID;
	struct BufferList *head, *tail;
	struct cdev dev;
	struct rwsemaphore *sem;
};

static dev_t majorNumber;
static int deviceCount = 1;
static int minor = 0;
static struct class *deviceClass;
static struct device *devices;
static struct Shared_Buffer *buffers;

#define SHARED_BUFFER_IOCTL 0x0f0f0f0f
#define SHARED_BUFFER_GET_SIZE _IOR(SHARED_BUFFER_IOCTL, 0, size_t)

module_param(deviceCount, int, S_IRUGO | S_IWUSR);


static int sharedBufferOpen(struct inode *nod, struct file *fp)
{
	struct Shared_Buffer *buf = container_of(nod->i_cdev, struct Shared_Buffer, dev);
	fp->private_data = buf;
	printk(KERN_INFO "Device %i has been opened\n", buf->deviceID);
	return 0;
}

static long int sharedBufferIOCTL(struct file *fp, unsigned int cmd, long unsigned int arg)
{
	printk(KERN_INFO "IOCTL call\n");
	
	if (cmd != SHARED_BUFFER_GET_SIZE)
	{
		return -ENOTTY;
	}	
	
	printk(KERN_INFO "IOCTL has been called with succsess\n");
	
	struct Shared_Buffer *buf = (struct Shared_Buffer*) fp->private_data;
	
	return copy_to_user((void __user *) arg, &buf->head->size, sizeof(size_t)) > 0 ? -EFAULT : 0; 
}

static ssize_t sharedBufferRead(struct file *fp, char __user *buf, size_t size, loff_t *off)
{	
	printk(KERN_INFO "Read call\n");
	struct Shared_Buffer *buffer = (struct Shared_Buffer*) fp->private_data;
	
	struct BufferList *iterator = buffer->head;
	
	if (iterator == NULL || size != iterator->size)
	{
		return 0;
	}
	
	size_t bytesReaded = iterator->size - copy_to_user(buf, iterator->buf, iterator->size);
	
	buffer->head = iterator->next;
	
	remove(iterator);
	return bytesReaded;
} 

static ssize_t sharedBufferWrite(struct file *fp, char const __user *buffer, size_t size, loff_t *off)
{
	struct Shared_Buffer *bufferEl = (struct Shared_Buffer*) fp->private_data;
	char *tmpBuf = (char*) kmalloc(size, GFP_KERNEL);
	int bytesCopied = size - copy_from_user(tmpBuf, buffer, size);
	struct BufferList *element = createElement(tmpBuf, bytesCopied); 
	
	if (bufferEl->head == NULL)
	{
		bufferEl->head = element;
		bufferEl->tail = element;
	}
	else
	{
		element->prev = bufferEl->tail;
		bufferEl->tail->next = element;
		bufferEl->tail = element;
	}
	
	bufferEl->totalSize += bytesCopied;
	return bytesCopied;
}

static int sharedBufferRelease(struct inode *nod, struct file *fp)
{
	return 0;
}


static struct file_operations SharedBufferFOPS = 
{
	.owner = THIS_MODULE,
	.open = sharedBufferOpen,
	.read = sharedBufferRead,
	.write = sharedBufferWrite,
	.release = sharedBufferRelease, 	
	.unlocked_ioctl = sharedBufferIOCTL,
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
	
	devices = (struct device*) kmalloc(sizeof(struct device) * deviceCount, GFP_KERNEL);
		
	int i = 0;
	for (; i < deviceCount; i++)
	{
		dev_t number = MKDEV(MAJOR(majorNumber), i);
		devices[i] = *(device_create(deviceClass, NULL, number, NULL, "SharedBuffer%i", i + 1));
	}
	
	
	if (devices == NULL)
	{
		class_destroy(deviceClass);
		unregister_chrdev_region(majorNumber, deviceCount);
		printk(KERN_ERR "Unable to register device\n");
		return 3;
	}
	
	buffers = (struct Shared_Buffer*) kmalloc(sizeof(struct Shared_Buffer) * deviceCount, GFP_KERNEL);	
	
	if (buffers == NULL)
	{
		printk(KERN_ERR "Failed to allocate memory\n");
	}
	
	i = 0;
	
	for (; i < deviceCount; i++)
	{
		cdev_init(&buffers[i].dev, &SharedBufferFOPS);
		buffers[i].dev.owner = THIS_MODULE;
		buffers[i].dev.ops = &SharedBufferFOPS;
		buffers[i].deviceID = i + 1;
		buffers[i].head = NULL;
		buffers[i].tail = NULL;
		dev_t number = MKDEV(MAJOR(majorNumber), i);
			
		if (cdev_add(&buffers[i].dev, number, 1))
		{
			printk(KERN_ERR "Can`t add cdev\n");
			return 4;
		}		
	}
	
	printk(KERN_INFO "Shared buffer\\s has\\ve been inited\n");
	printk(KERN_INFO "GET_SIZE IOCTL number: %xi\n",
		SHARED_BUFFER_GET_SIZE);
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
		i = 0;
		for (; i < deviceCount; i++)
		{
			dev_t number = MKDEV(MAJOR(majorNumber), i);
			device_destroy(deviceClass, number);
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
