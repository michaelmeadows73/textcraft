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
			if (command->child)
			{
				int result = !point_adjacent(entity->point, command->target) ? command->child->execute(command->child, entity, map) : 1;
				
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
			buildentity = map_get(map, cx, cy);
			if (buildentity == NULL)
			{
				if (entity->team->gold < 80)
				{
					team_setmessage(entity->team, "Not Enough Gold");
					return 1;
				}
				if (entity->team->wood < 40)
				{
					team_setmessage(entity->team, "Not Enough Wood");
					return 1;
				}
				if (entity->team->stone < 20)
				{
					team_setmessage(entity->team, "Not Enough Stone");
					return 1;
				}
				
				entity->team->gold -= 80;
				entity->team->wood -= 40;
				entity->team->stone -= 20;

				buildentity = entity_create(TYPE_FARM, SYMBOL_FARM);
				buildentity->point = command->target;
				buildentity->health = 1;
				buildentity->team = entity->team;
					
				map_set(map, cx, cy, buildentity);	
			}
			command->state = 2;
			break;
		case 2:
			buildentity = map_get(map, cx, cy);
			if (buildentity->type == TYPE_FARM && buildentity->team == entity->team)
			{
				// if not finished farm
				if (buildentity->health < 1001)
				{
					// increase farm 'health' - continue command
					buildentity->health += 5;
					return 0;
				}
				
				// if finished farm
				if (buildentity->health == 1001)
				{
					// ensure many peasants building one farm doesn't increase food many times
					buildentity->health = 1002;

					// increase food - exit command
					entity->team->food += 4;
					team_setmessage(entity->team, "Farm Complete");
					return 1;
				}
			
			}
			// not teams farm - exit command
			return 1;
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
