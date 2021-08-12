#include "LinkedBuffer.h"

struct BufferList *createSubBuffer(size_t size, struct BufferList *prev, struct BufferList *next)
{
	struct BufferList *buf = (struct BufferList*) kmalloc(sizeof(struct BufferList), GFP_USER);
	
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
