#ifndef COMMAND_H
#define COMMAND_H

#include "list.h"
#include "entity.h"
#include "team.h"

typedef int (*commandfn)();

struct command
{
	long target;
	struct entity* targetentity;
	struct list* path;
	struct command* child;
	int state;

	commandfn execute;
	char* desc;
};

void command_init(struct command*);

struct command* command_create();

void command_destroy(struct command*);

#endif
