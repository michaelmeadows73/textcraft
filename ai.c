#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "list.h"
#include "point.h"
#include "team.h"
#include "map.h"
#include "entity.h"
#include "peasant.h"
#include "castle.h"
#include "move.h"
#include "getwood.h"
#include "getstone.h"
#include "getgold.h"
#include "flee.h"
#include "attack.h"
#include "train.h"
#include "build.h"

struct entity* ai_idlepeasant(map, team)
struct map* map;
struct team* team;
{
	int x, y;
	for (y = 0; y < map->height; y++)
	{
		for (x = 0; x < map->width; x++)
		{
			struct entity* entity = map_get(map, x, y);
			if (entity && entity->type == TYPE_PEASANT && entity->team == team && entity->command == NULL)
			{
				return entity;
			}		
		}
	}
	return NULL;
}

int ai_randomresourcetype()
{
	int random = rand() % 3;
	int type;
	switch (random)
	{
		case 0:
			return TYPE_TREE;
		case 1:
			return TYPE_ROCK;
		case 2:
			return TYPE_MINE;
	}
}

void ai(map, team)
struct map* map;
struct team* team;
{
	struct entity* idlepeasant = ai_idlepeasant(map, team);
	if (idlepeasant != NULL)
	{
		int resourcetype = ai_randomresourcetype();
		
		long resourcepoint = map_find(map, resourcetype, NULL, idlepeasant->point);
		if (resourcepoint > -1)
		{
			switch (resourcetype)
			{
				case TYPE_TREE:
					idlepeasant->command = (struct command*) getwood_create(resourcepoint);
					break;
				case TYPE_ROCK:
					idlepeasant->command = (struct command*) getstone_create(resourcepoint);
					break;
				case TYPE_MINE:
					idlepeasant->command = (struct command*) getgold_create(resourcepoint);
					break;
			}
		}
	}
	else if (team->gold >= 60 && team->food >= map_count(map, TYPE_PEASANT, team))
	{
		long castlepoint = map_find(map, TYPE_CASTLE, team, point_create(0, 0));
		if (castlepoint > -1)
		{
			struct entity* castle = map_get(map, point_getx(castlepoint), point_gety(castlepoint));
			if (castle && castle->command == NULL)
			{
				castle->command = train_create();
			}
		}
	}
}
