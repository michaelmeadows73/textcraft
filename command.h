#ifndef COMMAND_H
#define COMMAND_H

#include "list.h"
#include "entity.h"
#include "team.h"

typedef int (*commandfn)();
typedef void (*collectfn)(struct team*);

struct command
{
	long target;
	struct entity* targetentity;
	struct list* path;
	struct command* child;
	int state;

	commandfn execute;

	int collecttype;
	collectfn collect;
};

struct command* command_create();

void command_destroy(struct command*);

#endif
