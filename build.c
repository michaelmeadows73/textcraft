#include <stdlib.h>
#include "team.h"
#include "map.h"
#include "entity.h"
#include "command.h"
#include "move.h"

int build_execute(command, entity, map)
struct command* command;
struct entity* entity;
struct map* map;
{
	int cx = point_getx(command->target);
	int cy = point_gety(command->target);
	struct entity* buildentity = NULL;
	switch (command->state)
	{
		case 0:
			if (entity->team->gold >= 80)
			{
				command->desc = "Taking Gold";

				entity->team->gold -= 80;

				command->state = 1;
			}
			else
			{
				command->desc = "Not Enough Gold";
			}
			break;
		case 1:
			if (entity->team->wood >= 40)
			{
				command->desc = "Taking Wood";

				entity->team->wood -= 40;

				command->state = 2;
			}
			else
			{
				command->desc = "Not Enough Wood";
			}
			break;
		case 2:
			if (entity->team->stone >= 20)
			{
				command->desc = "Taking Stone";

				entity->team->stone -= 40;

				command->state = 3;
			}
			else
			{
				command->desc = "Not Enough Stone";
			}
		case 3:
			if (map_get(map, cx, cy) == NULL)
			{
				buildentity = entity_create(TYPE_FARM, SYMBOL_FARM);
				buildentity->point = command->target;
				buildentity->health = 1;
				buildentity->team = entity->team;
					
				map_set(map, cx, cy, buildentity);				
			}
			command->state = 4;
			break;
		case 4:
			if (command->child)
			{
				int result = command->child->execute(command->child, entity, map);
				
				if (result)
				{
					command_destroy(command->child);
					command->child = NULL;
					command->state = 5;
				}
			}
			else
			{
				command->child = move_create(command->target);
			}
			break;
		case 5:
			command->desc = "Building Farm";
			
			buildentity = map_get(map, cx, cy);
			if (buildentity->health < 1000)
			{
				buildentity->health += 5;
			}
			else
			{
				command->state = 6;
				return 1;
			}
			break;		
	}
	return 0;
}

struct command* build_create(target)
long target;
{
	struct command* build = command_create();
	build->target = target;
	build->execute = build_execute;
	build->desc = "Building Farm";
	return build;
}
