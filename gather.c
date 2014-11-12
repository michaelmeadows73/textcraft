#include <stdlib.h>
#include "team.h"
#include "map.h"
#include "entity.h"
#include "command.h"
#include "gather.h"
#include "move.h"

int gather_execute(command, entity, map)
struct command* command;
struct entity* entity;
struct map* map;
{
	struct gather* gather = (struct gather*) command;

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
				treepoint = map_find(map, gather->collecttype, 0, command->target);
			
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
			if (mapentity && mapentity->type == gather->collecttype)
			{
				mapentity->health -= 10;

				if (mapentity->health <= 0)
				{
					entity_destroy(mapentity);
					map_set(map, cx, cy, NULL);
				}
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
			// collect resource
			gather->collect(entity->team);

			// get more resource
			command->state = 0;
			break;
	}
	return 0;
}

struct gather* gather_create(type, target, collecttype, collect, desc)
int type;
long target;
int collecttype;
collectfn collect;
char* desc;
{
	struct gather* gather = (struct gather*) malloc(sizeof(struct gather));
	struct command* command = (struct command*) gather;

	command_init(command);
	command->type = type;
	command->target = target;
	command->execute = gather_execute;
	command->desc = desc;

	gather->collecttype = collecttype;
	gather->collect = collect;
	return gather;
}
