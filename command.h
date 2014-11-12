#ifndef COMMAND_H
#define COMMAND_H

#include "list.h"
#include "entity.h"
#include "team.h"

#define COMMAND_MOVE		0
#define COMMAND_GETWOOD 	1
#define COMMAND_GETSTONE	2
#define COMMAND_GETGOLD 	3
#define COMMAND_BUILD		4
#define COMMAND_TRAIN		5
#define COMMAND_ATTACK		6
#define COMMAND_FLEE		7

typedef int (*commandfn)();

struct command
{
	int type;
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
