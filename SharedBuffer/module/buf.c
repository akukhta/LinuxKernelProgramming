#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/rwsem.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include "LinkedBuffer.h"
#include "bufioctl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexey Kukhta");

struct Shared_Buffer
{	
	size_t totalSize;
	size_t deviceID;
	struct BufferList *head, *tail;
	struct cdev dev;
	struct rw_semaphore sem;
	wait_queue_head_t wQueue; 
	bool isAvailable;
	size_t elementsCount;
};

static const char *semName = "sem";

static dev_t majorNumber;
static int deviceCount = 1;
static int minor = 0;
static struct class *deviceClass;
static struct device *devices;
static struct Shared_Buffer *buffers;

#define SHARED_BUFFER_IOCTL 0x0f0f0f0f
#define SHARED_BUFFER_GET_SIZE_OF_CURR_HEAD _IOR(SHARED_BUFFER_IOCTL, 0, size_t)
#define SHARED_BUFFER_GET_COUNT_OF_ELEMENTS _IOR(SHARED_BUFFER_IOCTL, 1, size_t)
#define SHARED_BUFFER_GET_TOTAL_SIZE _IOR(SHARED_BUFFER_IOCTL, 2, size_t);
#define SHARED_BUFFER_SET_NEW_HEAD _IOW(SHARED_BUFFER_IOCTL, 3, size_t);
#define SHARED_BUFFER_SET_STACK_MODE _IOW(SHARED_BUFFER_IOCTL, 4, bool);

module_param(deviceCount, int, S_IRUGO | S_IWUSR);


static int sharedBufferOpen(struct inode *nod, struct file *fp)
{
	struct Shared_Buffer *buf = container_of(nod->i_cdev, struct Shared_Buffer, dev);
	fp->private_data = buf;
	printk(KERN_INFO "Device %i has been opened\n", buf->deviceID);
	return 0;
}

static unsigned int sharedBufferPoll(struct file *filep, poll_table *wait)
{
	unsigned int mask = 0;
	
	struct Shared_Buffer *buf = (struct Shared_Buffer *) filep->private_data;
	poll_wait(filep, &buf->wQueue, wait);
	
	mask |= POLLIN | POLLRDNORM | POLLWRNORM | POLLOUT;
	
	return mask;
	
}

static long int sharedBufferIOCTL(struct file *fp, unsigned int cmd, long unsigned int arg)
{
	if (cmd != SHARED_BUFFER_GET_SIZE_OF_CURR_HEAD)
	{
		return -ENOTTY;
	}

	struct Shared_Buffer *buf = (struct Shared_Buffer *) fp->private_data;

	down_read(&buf->sem);
	wait_event_interruptible(buf->wQueue, buf->isAvailable);
	up_read(&buf->sem);

	return copy_to_user((void __user *) arg, &buf->head->size, sizeof(size_t)) > 0 ? -EFAULT : 0; 
}

static ssize_t sharedBufferRead(struct file *fp, char __user *buf, size_t size, loff_t *off)
{	
	printk(KERN_INFO "Read call\n");
	struct Shared_Buffer *buffer = (struct Shared_Buffer*) fp->private_data;
	
	down_read(&buffer->sem);
	wait_event_interruptible(buffer->wQueue, buffer->isAvailable == true);

	struct BufferList *iterator = buffer->head;
	
	if (iterator == NULL || size != iterator->size)
	{
		return 0;
	}
	
	size_t bytesReaded = iterator->size - copy_to_user(buf, iterator->buf, iterator->size);
	
	buffer->head = iterator->next;
	remove(iterator);
	buffer->elementsCount--;
	up_read(&buffer->sem);
	
	return bytesReaded;
} 

static ssize_t sharedBufferWrite(struct file *fp, char const __user *buffer, size_t size, loff_t *off)
{
	struct Shared_Buffer *bufferEl = (struct Shared_Buffer*) fp->private_data;
	char *tmpBuf = (char*) kmalloc(size, GFP_KERNEL);
	int bytesCopied = size - copy_from_user(tmpBuf, buffer, size);
	struct BufferList *element = createElement(tmpBuf, bytesCopied); 
	
	down_write(&bufferEl->sem);
	bufferEl->isAvailable = false;
	up_write(&bufferEl->sem);
	
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
	bufferEl->elementsCount++;
	bufferEl->isAvailable = true;
	wake_up_interruptible(&bufferEl->wQueue);
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
		init_rwsem(&buffers[i].sem);
		init_waitqueue_head(&buffers[i].wQueue);
		buffers[i].isAvailable = true;
		buffers[i].head = NULL;
		buffers[i].tail = NULL;
		buffers[i].elementsCount = 0;
		dev_t number = MKDEV(MAJOR(majorNumber), i);
			
		if (cdev_add(&buffers[i].dev, number, 1))
		{
			printk(KERN_ERR "Can`t add cdev\n");
			return 4;
		}		
	}
	
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
