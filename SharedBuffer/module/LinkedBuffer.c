#include "LinkedBuffer.h"

struct BufferList* createElement(char const * data, size_t size)
{
	printk(KERN_INFO "Creating the element\n");
	struct BufferList *buf = (struct BufferList*) kmalloc(sizeof(struct BufferList), GFP_KERNEL);
	buf->buf = (char*) kmalloc(size, GFP_KERNEL);
	memcpy(buf->buf, data, size);
	buf->size = size;
	buf->prev = NULL;
	buf->next = NULL;
	printk(KERN_INFO "The element has been created\n");
	return buf;
}

struct BufferList *createSubBuffer(size_t size, struct BufferList *prev, struct BufferList *next)
{
	struct BufferList *buf = (struct BufferList*) kmalloc(sizeof(struct BufferList), GFP_KERNEL);
	
	if (buf == NULL)
	{
		return NULL;
	}
	
	buf->size = size;
	buf->buf = (char*) kmalloc(size, GFP_KERNEL);
	
	if (buf->buf == NULL)
	{
		return NULL;
	}
	
	buf->prev = prev;
	buf->next = next;
	
	return buf;

}

int add_back(struct BufferList *back, char const * data, size_t size)
{
	struct BufferList *buf = createElement(data, size);
	buf->prev = back;
	back->next = buf;
	back = buf;
	return 0;
}

int insert(struct BufferList* prev, char const * data, size_t size)
{
	struct BufferList *buf = createElement(data, size);
	buf->next = prev->next;
	prev->next = buf;
	buf->prev = prev;
	return 0;
}


int add_begin(struct BufferList *begin, char const * data, size_t size)
{
	struct BufferList *buf = createElement(data, size);
	buf->next = begin;
	begin->prev = buf;
	begin = buf;
	return 0;
}

int remove(struct BufferList *item)
{
	if (item == NULL)
	{
		return 0;
	}
	
	printk(KERN_INFO "Removing the element\n");
	
	if (item->next != NULL)
	{
		item->next->prev = NULL;
	}
	
	kfree(item->buf);
	kfree(item);
	item = NULL;
	printk(KERN_INFO "The element has been removed\n");
	return 0;
}
