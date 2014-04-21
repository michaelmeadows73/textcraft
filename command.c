#include <stdlib.h>
#include "team.h"
#include "command.h"

struct command* command_create()
{
	struct command* command = (struct command*) malloc(sizeof(struct command));
	command->target = 0;
	command->path = NULL;
	command->child = NULL;
	command->state = 0;
	command->execute = NULL;
	
	command->collecttype = 0;
	command->collect = NULL;
	return command;
}

void command_destroy(command)
struct command* command;
{
	if (command->path)
	{
		list_destroy(command->path);
		command->path = NULL;
	}

	if (command->child)
	{
		command_destroy(command->child);
		command->child = NULL;
	}
	
	free(command);
}
