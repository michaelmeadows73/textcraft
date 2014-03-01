#include <stdlib.h>
#include <stdio.h>
#include "list.h"

struct link* link_create()
{
	struct link* link = (struct link*) malloc(sizeof(struct link));
	link->next = NULL;
	link->previous = NULL;
	link->data = NULL;
	return link;
}

void link_destroy(link)
struct link* link;
{
	free(link);
}

struct list* list_create()
{
	struct list* list = (struct list*) malloc(sizeof(struct list));
	list->first = NULL;
	list->last = NULL;
	list->count = 0;
	return list;
}

void list_add(list, data)
struct list* list;
void* data;
{
	struct link* link = link_create();
	link->data = data;

	if (list->first == NULL)
	{
		list->first = link;
	}
	if (list->last)
	{
		list->last->next = link;
		link->previous = list->last;
	}

	list->last = link;
	list->count++;
}

int list_count(list)
struct list* list;
{
	return list->count;
}

int list_empty(list)
struct list* list;
{
	return list->count == 0;
}

void* list_getfirst(list)
struct list* list;
{
	return list->first ? list->first->data : NULL;
}

void* list_getitem(list, index)
struct list* list;
{
	if ((index < 0) || (index >= list->count))
	{
		return NULL;
	}

	struct link* link = list->first;

	int i;
	for (i = 1; i <= index; i++)
	{
		link = link->next;
	}

	return link->data;
}

void* list_getlast(list)
struct list* list;
{
	return list->last ? list->last->data : NULL;
}

void* list_find(list, predicate, predicatestate)
struct list* list;
int predicate();
void* predicatestate;
{
	struct link* link = list->first;
	while (link)
	{
		if (predicate(link->data, predicatestate))
		{
			return link->data;
		}
		link = link->next;
	}
	return NULL;
}

void list_iterate(list, iterator, iteratorstate)
struct list* list;
void iterator();
void* iteratorstate;
{
	struct link* link = list->first;
	while (link)
	{
		iterator(link->data, iteratorstate);
		link = link->next;
	}
}

struct list* list_clone(list)
struct list* list;
{
	struct list* clone = list_create();

	struct link* link = list->first;
	while (link)
	{
		list_add(clone, link->data);		
		link = link->next;
	}

	return clone;
}

int list_contains(list, data, equals)
struct list* list;
void* data;
int equals();
{
	struct link* link = list->first;
	while (link)
	{
		if (equals(link->data, data))
		{
			return 1;
		}
		link = link->next;
	}

	return 0;
}

void list_removefirst(list)
struct list* list;
{
	if (list->first)
	{
		struct link* next = list->first->next;
		link_destroy(list->first);
		list->first = next;

		if (list->first)
		{
			list->first->previous = NULL;
			if (list->first->next == NULL)
			{
				list->last = list->first;
			}
		}
		else
		{
			list->last = NULL;
		}

		list->count--;
	}
}

void list_removelast(list)
struct list* list;
{
	if (list->last)
	{
		struct link* previous = list->last->previous;
		link_destroy(list->last);
		list->last = previous;

		if (list->last)
		{
			list->last->next = NULL;
			if (list->last->previous == NULL)
			{
				list->first = list->last;
			}
		}
		else
		{
			list->first = NULL;
		}

		list->count--;			
	}
}

void list_destroy(list)
struct list* list;
{
	struct link* link = list->first;
	
	while (link)
	{
		struct link* next = link->next;
		link_destroy(link);
		link = next;
	}

	free(list);
}
