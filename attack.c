#include <stdlib.h>
#include "team.h"
#include "map.h"
#include "entity.h"
#include "command.h"
#include "move.h"
#include "flee.h"

struct command* attack_create(long);

int normalize(int d)
{
	return d == 0 ? 0 : (d > 0 ? 1 : -1);
}

int attack_execute(command, entity, map)
struct command* command;
struct entity* entity;
struct map* map;
{
	long enemypoint, castlepoint;
	struct entity* mapentity;

	int tx, ty;
	struct entity* tentity;
	switch (command->state)
	{
		case 0:
			tx = point_getx(command->target);
			ty = point_gety(command->target);
			tentity = map_get(map, tx, ty);
			if (tentity != NULL)
			{
				if (tentity->team == entity->team->enemy)
				{
					command->targetentity = tentity;
					command->state = 1;
				}
				else
				{
					// cannot attack units on own team
					return 1;
				} 
			}
			else
			{
				enemypoint = map_find(map, TYPE_NONE, entity->team->enemy, command->target);
				if (enemypoint != -1)
				{
					command->targetentity = map_get(map, point_getx(enemypoint), point_gety(enemypoint));
					command->state = 1;
				}
				else
				{
					// cannot find enemy unit to attack
					return 1;
				}
			}
			break;		
		case 1:
			if (point_adjacent(entity->point, command->targetentity->point))
			{
				command->targetentity->health = command->targetentity->health - rand() % 10;

				// if target entity is a live peasant
				if (command->targetentity->health > 0)
				{
					// if target entity can attack or flee
					if (command->targetentity->type == TYPE_PEASANT)
					{
						
						// if target entity is not fleeing and has low health - stop
						if (command->targetentity->command && command->targetentity->command->execute != flee_execute && command->targetentity->health <= 33)
						{
							command_destroy(command->targetentity->command);
							command->targetentity->command = NULL;
						}

						// if target entity is not attacking and has high health - stop
						if (command->targetentity->command && command->targetentity->command->execute != attack_execute && command->targetentity->health > 33) 
						{
							command_destroy(command->targetentity->command);
							command->targetentity->command = NULL;
						}

						// if target entity has stopped
						if (command->targetentity->command == NULL)
						{
							if (command->targetentity->health > 33)
							{
								// high health - attack
								command->targetentity->command = attack_create(entity->point);
							}
							else
							{
								// low health - flee
								command->targetentity->command = flee_create();
							}
						}

					}
				}
				else
				{
					command->target = entity->point;
					command->targetentity = NULL;
					command->state = 2;
				}
			}
			else
			{
				int px = point_getx(entity->point);
				int py = point_gety(entity->point);
				int tx = point_getx(command->targetentity->point);
				int ty = point_gety(command->targetentity->point);

				int dx = normalize(tx - px);
				int dy = normalize(ty - py);
			
				map_set(map, px, py, NULL);
				if (dx != 0)
				{
					int pxdx = px + dx;
					if (map_get(map, pxdx, py) == NULL)
					{
						px = pxdx;
					}
				}
				if (dy != 0)
				{
					int pydy = py + dy;
					if (map_get(map, px, pydy) == NULL)
					{
						py = pydy;
					}
				}
				map_set(map, px, py, entity);
				
				entity->point = point_create(px, py);
			}
			break;
		case 2:
			return 1;
	}
	return 0;
}

struct command* attack_create(target)
long target;
{
	struct command* command = command_create();
	command->target = target;
	command->path = NULL;
	command->child = NULL;
	command->state = 0;
	command->execute = attack_execute;
	command->desc = "Attacking";
	return command;
}
