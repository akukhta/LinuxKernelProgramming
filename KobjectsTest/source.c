#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");

static struct kobject obj;

static int __init load(void)
{
	printk(KERN_INFO "Module has been loaded\n");
	kobject_init(&obj);
	
	if (kobject_set_name(&obj, "MYOBJECT"))
	{
		printk(KERN_ERR "Failed to set a name to kobject\n");
		return 1;
	}	
	
	return 0;
}

static void __exit exit(void)
{
	printk(KERN_INFO "Module has been unloaded\n");
	return;
}

module_init(load);
module_exit(exit);

