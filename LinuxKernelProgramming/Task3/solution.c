#include <linux/module.h>
#include <linux/init.h>
#include <linux/random.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

extern int array_sum(short *, size_t);
extern ssize_t generate_output(int, short *, size_t , char *);
static short* getRandomArray(size_t *);
static void test(void);
static int trusted_sum(short * const, size_t);
static unsigned int cyclesCount = 0;
module_param(cyclesCount, uint, S_IRUGO | S_IWUSR);

static int __init my_module_init(void)
{
	test();
	return 0;
}

static void __exit my_module_exit(void)
{
//	CHECKER_MACRO();
	return;
}

module_init(my_module_init);
module_exit(my_module_exit);

void test(void)
{
	if (!cyclesCount)
	{
		get_random_bytes(&cyclesCount, sizeof(cyclesCount));
		cyclesCount %= 100;
	}

	size_t i = 0;

	for (; i < cyclesCount; i++)
	{
		size_t arrayCount;
		short *array = getRandomArray(&arrayCount);
		
		if (array == NULL)
		{
			return;
		}
		
		int tSum = trusted_sum(array, arrayCount);
		int sum = array_sum(array, arrayCount);
		char * buf = (char*) kmalloc(1000, GFP_ATOMIC);
		generate_output(sum, array, arrayCount, buf);
		if (sum == tSum)
		{
			printk(KERN_INFO "%s", buf);
		}
		else
		{
			printk(KERN_ERR "%s", buf);
		}
		
		kfree(buf);
		kfree(array);
	} 
}

short* getRandomArray(size_t *size)
{
	get_random_bytes(size, sizeof(*size));
	
	
	(*size) %= 100;
	
	short *dst = (short*) kmalloc((*size) * sizeof(short), GFP_ATOMIC);
	
	if (!dst)
	{
		return NULL;
	}
	
	
	size_t i = 0;
	
	for (; i < *size; i++)
	{
		get_random_bytes(&dst[i], sizeof(dst[i]));
	}
	
	return dst;
}

static int trusted_sum(short * const arr, size_t n)
{
	if (!arr)
	{
		return 0;
	}
	
	int sum = 0;
	size_t i = 0;
	
	for (; i < n; i++)
	{
		sum += (int) arr[i];
	}
	
	return sum;
}
