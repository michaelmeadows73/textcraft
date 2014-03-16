#include <stdlib.h>
#include "map.h"
#include "entity.h"
#include "command.h"
#include "move.h"

int getwood_execute(command, entity, map)
struct command* command;
struct entity* entity;
struct map* map;
{
	int cx, cy;
	long treepoint, castlepoint;
	switch (command->state)
	{
		case 0:
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
			break;
		case 1:
			if (command->child)
			{
				int result = command->child->execute(command->child, entity, map);
			
				if (result)
				{
					move_destroy(command->child);
					command->child = NULL;
					command->state = 2;
				}
			}
			else
			{
				treepoint = map_find(map, 'T', entity->point);
			
				if (treepoint != -1)
				{
					command->target = treepoint;
					command->child = move_create(treepoint);
				}
				else
				{
					return 1;
				}
			}
			break;
		case 2:
			cx = point_getx(command->target);
			cy = point_gety(command->target);
			if (map_get(map, cx, cy) == 'T')
			{
				map_set(map, cx, cy, ' ');
				command->state = 3;
			}
			else
			{
				command->state = 1;
			}
			break;
		case 3:
			if (command->child)
			{
				int result = command->child->execute(command->child, entity, map);

				if (result)
				{
					// arrived at castle
					move_destroy(command->child);
					command->child = NULL;
					command->state = 4;
				}
			}
			else
			{
				castlepoint = map_find(map, 'C', entity->point);
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
			break;
		case 4:
			// increment wood somewhere

			// get more wood
			command->state = 0;
			break;
	}
	return 0;
}

struct command* getwood_create(target)
long target;
{
	struct command* command = command_create();
	command->target = target;
	command->path = NULL;
	command->child = NULL;
	command->state = 0;
	command->execute = getwood_execute;
	return command;
}

void getwood_destroy(getwood)
struct command* getwood;
{
	command_destroy(getwood);
}
