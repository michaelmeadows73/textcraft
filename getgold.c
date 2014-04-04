#include <stdlib.h>
#include "map.h"
#include "entity.h"
#include "command.h"
#include "move.h"

int getgold_execute(command, entity, map)
struct command* command;
struct entity* entity;
struct map* map;
{
	int cx, cy;
	long castlepoint;
	struct entity* mapentity;
	if (command->state == 0)
	{
		// go to mine
		if (command->child)
		{
			int result = command->child->execute(command->child, entity, map);
			
			if (result)
			{
				move_destroy(command->child);
				command->child = NULL;
				command->state = 1;
			}
		}
		else
		{
			command->child = move_create(command->target);
		}
	}
	else if (command->state > 0 && command->state < 100)
	{
		// mining
		command->state++;
	}
	else if (command->state == 100)
	{
		// go to castle
		if (command->child)
		{
			int result = command->child->execute(command->child, entity, map);

			if (result)
			{
				// arrived at castle
				move_destroy(command->child);
				command->child = NULL;
				command->state = 101;
			}
		}
		else
		{
			castlepoint = map_find(map, TYPE_CASTLE, entity->team, entity->point);
			if (castlepoint != -1)
			{
				// found castle - go to castle
				command->child = move_create(castlepoint);
			}
			else
			{
				// not found castle - cancel
				return 1;
			}
		}
	}
	else if (command->state == 101)
	{
		// drop gold

		// go back to mine
		command->state = 0;
	}

	return 0;
}

struct command* getgold_create(target)
long target;
{
	struct command* command = command_create();
	command->target = target;
	command->path = NULL;
	command->child = NULL;
	command->state = 0;
	command->execute = getgold_execute;
	return command;
}

void getgold_destroy(getgold)
struct command* getgold;
{
	command_destroy(getgold);
}
