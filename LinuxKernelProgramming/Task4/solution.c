#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>


//Pointers
static void *data;
static int *int_array;
static struct device *dev;

//External functions to: 
//1. Get size
//2. Validate allocation of memory
//3. Free allocated memory
extern ssize_t get_void_size(void);
extern void submit_void_ptr(void *p);
extern ssize_t get_int_array_size(void);
extern void submit_int_array_ptr(int *p);
extern void submit_struct_ptr(struct device *p);
extern void checker_kfree(void *p);

static int __init solution_init(void)
{
	ssize_t size = get_void_size();
	data = kmalloc(size, GFP_KERNEL);
	submit_void_ptr(data);
	
	size = get_int_array_size();
	int_array = (int*) kmalloc(size * sizeof(int), GFP_KERNEL);
	submit_int_array_ptr(int_array);
	
	dev = (struct device*) kmalloc(sizeof(struct device), GFP_KERNEL);
	submit_struct_ptr(dev);	
	return 1;
}

static void __exit solution_cleanup(void)
{
	checker_kfree(data);
	checker_kfree(int_array);
	checker_kfree(dev);
}

module_init(solution_init);
module_exit(solution_cleanup);
