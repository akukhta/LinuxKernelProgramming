#include <linux/module.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR("Me");

static int __init hello_init(void)
{
    printk("Loading!");
    return 0;
}

static void __exit hello_exit(void)
{
    printk("Leaving!");
}

module_init(hello_init);
module_exit(hello_exit);
