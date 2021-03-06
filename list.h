#ifndef LIST_H
#define LIST_H

struct link
{
	struct link* next;
	struct link* previous;
	void* data;
};

struct list
{
	struct link* first;
	struct link* last;
	int count;
};

struct list* list_create();

void list_add(struct list*, void*);

void list_prepend(struct list*, void*);

int list_count(struct list*);

int list_empty(struct list*);

void* list_getfirst(struct list*);

void* list_getitem(struct list*, int);

void* list_getlast(struct list*);

void* list_find(struct list*, int(), void*);

void list_iterate(struct list*, void(), void*);

struct list* list_clone(struct list*);

int list_contains(struct list*, void*, int());

void list_removefirst(struct list*);

void list_removelast(struct list*);

void list_clear(struct list*);

void list_destroy(struct list*);

#endif
