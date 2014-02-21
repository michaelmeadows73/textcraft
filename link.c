#include <stdlib.h>
#include "link.h"

struct link* link_last(link)
struct link* link;
{
	while (link->next) 
	{
		link = link->next;
	}
	return link;
}

struct link* link_add(head, data)
struct link* head;
void* data;
{
	struct link* link = (struct link*) malloc(sizeof(struct link));
	link->next = NULL;
	link->data = data;

	if (head)
	{
		struct link* last = link_last(head);
		last->next = link;
	}

	return link;
}

struct link* link_clone(link)
struct link* link;
{
	struct link* clone_head = link_add(NULL, link->data);
	struct link* clone = clone_head;
	while (link->next)
	{
		link = link->next;
		clone = link_add(clone, link->data);
	}
	return clone_head;
}

int link_contains(link, data, equals)
struct link* link;
void* data;
int equals();
{
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
