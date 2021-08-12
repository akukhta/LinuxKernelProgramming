#include "LinkedBuffer.h"

struct BufferList* createElement(char const * data, size_t size)
{
	struct BufferList *buf = (struct BufferList*) kmalloc(sizeof(struct BufferList), GFP_USER);
	buf->buf = (char*) kmalloc(size, GFP_USER);
	memcpy(buf->buf, data, size);
	buf->size = size;
	return buf;
}

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
