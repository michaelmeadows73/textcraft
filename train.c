#include <stdio.h>
#include "command.h"
#include "entity.h"
#include "map.h"
#include "peasant.h"

int train_execute(command, entity, map)
struct command* command;
struct entity* entity;
struct map* map;
{
	if (command->state == 0)
	{
		if (entity->team->gold < 60)
		{
			command->desc = "Not Enough Gold";
			return 0;
		}
		if (entity->team->food <= map_count(map, TYPE_PEASANT, entity->team))
		{
			command->desc = "Not Enough Food";
			return 0;
		}
	
		command->desc = "Taking Gold";	
		
		entity->team->gold -= 60;
		
		command->state = 1;
	}
	if (command->state >= 1 && command->state < 100)
	{
		command->desc = "Training Peasant";

		command->state++;
	}
	if (command->state >= 100)
	{
		command->desc = "Releasing Peasant";

		int ex = point_getx(entity->point);
		int ey = point_gety(entity->point);

		int dx, dy;
		for (dy = -1; dy <= 1; dy++)
		{
			for (dx = -1; dx <= 1; dx++)
			{
				if (dx != 0 || dy != 0)
				{
					int px, py;
					px = ex + dx;
					py = ey + dy;

					if (px >= 0 && px <= map->width && py >= 0 && py <= map->height)
					{
						if (map_get(map, px, py) == NULL)
						{
							long point = point_create(px, py);
							struct entity* peasant = peasant_create(point);
							peasant->team = entity->team;
							map_set(map, px, py, peasant);
							return 1;
						}
					}				
				}
			}
		}
	}
	return 0;
}

struct command* train_create()
{
	struct command* command = command_create();
	command->target = 0;
	command->path = NULL;
	command->child = NULL;
	command->state = 0;
	command->execute = train_execute;
	command->desc = "Training Peasant";
	return command;
}
