#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/moduleparam.h>
#include <linux/uaccess.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");

ssize_t read_function(struct file *, char *, size_t, loff_t *);
ssize_t write_function(struct file *, const __user char *, size_t, loff_t *);
 
static bool isReaded = false;

//This message returns from read function
//You can pass it as parameter during insmod 
//Or modify it on: /sys/module/sparrow/parameters/msg
static char *msg = "Message from sparrow!";
module_param(msg, charp, S_IRUGO | S_IWUSR);

static struct proc_ops fops = 
{
	.proc_read = read_function,
	.proc_write = write_function,
};

static int __init sparrow_init(void)
{
	printk(KERN_INFO "sparrow has been loaded!\n");
	proc_create("sparrow", 0, NULL, &fops);
	return 0;
}

static void __exit sparrow_exit(void)
{
	printk(KERN_INFO "sparrow has been unloaded!\n");
	remove_proc_entry("sparrow", NULL);
}


ssize_t read_function(struct file *filep, char __user *buf, size_t count, loff_t *offp)
{
	printk(KERN_INFO "sparrow is talking\n");
	sprintf(buf, msg);

	if (!isReaded)
	{
		isReaded = true;
		return strlen(msg) + 1;
	}
	else
	{
		isReaded = false;
		return 0;
	}
}

ssize_t write_function(struct file * filep, const __user char * buf, size_t count, loff_t *offp)
{
	printk(KERN_INFO "sparrow is listening\n");
	printk(KERN_INFO "count: %u\n", count);
	memset(msg,0, strlen(msg));
	copy_from_user(msg, buf, count);
	return count;
}

module_init(sparrow_init);
module_exit(sparrow_exit);
