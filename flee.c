#include <stdlib.h>
#include "command.h"
#include "move.h"
#include "entity.h"
#include "map.h"
#include "point.h"

// some comment

int flee_execute(command, entity, map)
struct command* command;
struct entity* entity;
struct map* map;
{
	int r = 20;
	int ex, ey;
	int fx, fy;
	switch (command->state)
	{
		case 0:
			ex = point_getx(entity->point);
			ey = point_gety(entity->point);

			while (1)
			{
				fx = ex + rand() % (2 * r) - r;
				fy = ey + rand() % (2 * r) - r;

				if ((fx > 0) && (fx < map->width) && (fy > 0) && (fy < map->height) && map_get(map, fx, fy) == NULL)
				{
					command->child = move_create(point_create(fx, fy));
					if (command->child->execute(command, entity, map) != -1)
					{
						command->state = 1;
						break;
					}
					else
					{
						command_destroy(command->child);
					}
				}
			}
			break;
		case 1:
			if (command->child->execute(command->child, entity, map) == 1)
			{
				command_destroy(command->child);
				command->child = NULL;
				command->state = 2;
				return 1;
			}
			break;
			
	}
	return 0;
}

struct command* flee_create()
{
	struct command* command = command_create();
	command->type = COMMAND_FLEE;
	command->target = 0;
	command->path = NULL;
	command->child = NULL;
	command->state = 0;
	command->execute = flee_execute;
	command->desc = "Fleeing";
	return command;
}
