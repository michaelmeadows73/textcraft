#include <stdlib.h>
#include "team.h"
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
	struct entity* mapentity;
	switch (command->state)
	{
		case 0:
			if (command->child)
			{
				int result = command->child->execute(command->child, entity, map);
				
				if (result)
				{
					command_destroy(command->child);
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
					command_destroy(command->child);
					command->child = NULL;
					command->state = 2;
				}
			}
			else
			{
				treepoint = map_find(map, command->collecttype, 0, command->target);
			
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
			mapentity = map_get(map, cx, cy);
			if (mapentity && mapentity->type == command->collecttype)
			{
				entity_destroy(mapentity);
				
				map_set(map, cx, cy, NULL);
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

				if (result == 1)
				{
					// arrived at castle
					command_destroy(command->child);
					command->child = NULL;
					command->state = 4;
				}
				if (result == -1)
				{
					// can't get to castle - cancel
					return 1;
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
			break;
		case 4:
			// increment wood
			command->collect(entity->team);

			// get more wood
			command->state = 0;
			break;
	}
	return 0;
}

void getwood_collect(team)
struct team* team;
{
	team->wood += 10;
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

	command->collecttype = TYPE_TREE;
	command->collect = getwood_collect;

	return command;
}

void getstone_collect(team)
struct team* team;
{
	team->stone += 10;
}

struct command* getstone_create(target)
long target;
{
	struct command* command = command_create();
	command->target = target;
	command->path = NULL;
	command->child = NULL;
	command->state = 0;
	command->execute = getwood_execute;

	command->collecttype = TYPE_ROCK;
	command->collect = getstone_collect;

	return command;

}
