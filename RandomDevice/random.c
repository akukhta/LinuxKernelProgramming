#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

MODULE_LICENSE("GPL");

static const char deviceName[] = "RandomDevice";
static dev_t majorNumber;
static int deviceCount = 1;
static int minor = 0;
static struct class *devClass;
static struct device *devDevice;
static struct cdev *myCdev;
module_param(deviceCount, int, S_IRUGO | S_IWUSR); 

static int randomOpen(struct inode *in, struct file * f)
{
	printk(KERN_INFO "The random device has been opened via %i:%i\n", MAJOR(in->i_rdev), MINOR(in->i_rdev));
	return 0;
}

static int randomRelease(struct inode *in, struct file * f)
{
	printk(KERN_INFO "The random device has been released via %i:%i\n", MAJOR(in->i_rdev), MINOR(in->i_rdev));
	return 0;
}

static ssize_t randomRead(struct file *f, char __user *userBuf, size_t count, loff_t *off)
{
	char *tmp_buf = (char*) kmalloc(count, GFP_KERNEL);
	
	if (!tmp_buf)
	{
		printk(KERN_ERR "Memory allocation has been failed\n");
		return 0;
	}
	
	get_random_bytes(tmp_buf, count);
	int bytes = count - copy_to_user(userBuf, tmp_buf, count);
	kfree(tmp_buf);
	return bytes;	
}

static const struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = randomOpen,
	.release = randomRelease,
	.read = randomRead,
};

static int __init init_devices(void)
{
	if (alloc_chrdev_region(&majorNumber, minor, deviceCount, deviceName))
	{
		printk(KERN_ERR "Numbers' allocation has been failed\n");
		return 1;
	}
	
	devClass = class_create(THIS_MODULE, deviceName);
	
	if (devClass == NULL)
	{
		printk(KERN_ERR "Can not create a device's class\n");
		unregister_chrdev_region(majorNumber, deviceCount);
		return 1;
	}
	
	if (deviceCount == 1)
	{
		devDevice = device_create(devClass, NULL, majorNumber, NULL, deviceName);
	}
	
	else
	{
		devDevice = (struct device*) kmalloc(sizeof(struct device) * deviceCount, GFP_KERNEL);
		
		int i = 0;
		for (; i < deviceCount; i++)
		{
			dev_t number = MKDEV(MAJOR(majorNumber), i);
			devDevice[i] = *(device_create(devClass, NULL, number, NULL, "RandomDevice%i", i + 1));
			printk(KERN_INFO "%i,%i device has been inited\n", MAJOR(number), MINOR(number));
		}
	}
	
	if (devDevice == NULL)
	{
		class_destroy(devClass);
		unregister_chrdev_region(majorNumber, deviceCount);
		printk(KERN_ERR "Unable to register device\n");
	}
	
	myCdev = cdev_alloc();
	cdev_init(myCdev, &fops);
	cdev_add(myCdev, majorNumber, deviceCount);
	
	printk(KERN_INFO "The random device has been inited\n");
	return 0;	
}

static void __exit clean_devices(void)
{
	if (myCdev != NULL)
	{
		cdev_del(myCdev);
	}
	
	if (devClass != NULL)
	{	
		if (deviceCount == 1)
		{
			device_destroy(devClass, majorNumber);
		}
		else
		{
			int i = 0;
			for (; i < deviceCount; i++)
			{
				dev_t number = MKDEV(MAJOR(majorNumber), i);
				device_destroy(devClass, number);
			}
		}
	}
	
	if (devClass != NULL)
	{
		class_destroy(devClass);
	}
	
	unregister_chrdev_region(majorNumber, deviceCount);
	printk(KERN_INFO "The random device has been cleaned\n");
	return;
}

module_init(init_devices);
module_exit(clean_devices);
