#include <stdlib.h>
#include "command.h"

struct command* command_create()
{
	struct command* command = (struct command*) malloc(sizeof(struct command));
	command->target = 0;
	command->path = NULL;
	command->execute = NULL;
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

	free(command);
}
