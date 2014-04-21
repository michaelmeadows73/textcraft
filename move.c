#include <stdlib.h>
#include "point.h"
#include "entity.h"
#include "map.h"
#include "command.h"
#include "move.h"

int move_execute(command, entity, map)
struct command* command;
struct entity* entity;
struct map* map;
{
	if (command->target >= 0 && command->path == NULL)
	{
		command->path = map_shortestpath(map, entity->point, command->target);
		if (command->path == NULL)
		{
			// can't get to target
			command->target = -1;
			return -1;
		}
	}

	if (command->path)
	{
		if (!list_empty(command->path))
		{
			long nextpoint = (long) list_getfirst(command->path);
			if (map_get(map, point_getx(nextpoint), point_gety(nextpoint)) == NULL)
			{
				// no obstacle - move
				entity->point = nextpoint;
				list_removefirst(command->path);
			}
			else
			{
				// found obstacle - recalculate path
				list_destroy(command->path);
				command->path = NULL;
			}
		}
		else
		{
			// finished path - stop
			list_destroy(command->path);
			command->path = NULL;
			command->target = -1;
			return 1;
		}
	}
	
	return 0;
}

struct command* move_create(target)
long target;
{
	struct command* command = command_create();
	command->target = target;
	command->path = NULL;
	command->child = NULL;
	command->state = 0;
	command->execute = move_execute;
	return command;
}

void move_destroy(move)
struct command* move;
{
	command_destroy(move);
} 
