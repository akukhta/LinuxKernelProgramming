#include <linux/module.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

struct BufferList
{
	char *buf;
	size_t size;
	struct BufferList *next;
	struct BufferList *prev;
};

struct BufferList* createSubBuffer(size_t size, struct BufferList *prev, struct BufferList *next);

EXPORT_SYMBOL(createSubBuffer);
