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

//Creates queue
struct BufferList* createSubBuffer(size_t size, struct BufferList *prev, struct BufferList *next);
//Add data to end
int add_back(struct BufferList *back, char *data, size_t size);
//Add data to begin
int add_begin(struct BufferList *begin, char const * data, size_t size);
//Insert
int insert(struct BufferList* prev,  char const * data, size_t size);
//Find i element of queue


//TODO Next:
/*
struct BufferList* find(struct BufferList *head, size_t i);
//Remove i element of queue
int remove(struct BufferList *head, size_t i); 
//Remove iterator
int remove(struct BufferList *item);
*/

//Helping function to allocate element
struct BufferList* createElement(char const * data, size_t size);

EXPORT_SYMBOL(createSubBuffer);
EXPORT_SYMBOL(add_back);
EXPORT_SYMBOL(add_begin);
EXPORT_SYMBOL(insert);
