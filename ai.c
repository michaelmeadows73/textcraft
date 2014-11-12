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

struct entity* ai_buildpeasant(map, team)
struct map* map;
struct team* team;
{
	int x, y;
	for (y = 0; y < map->height; y++)
	{
		for (x = 0; x < map->width; x++)
		{
			struct entity* entity = map_get(map, x, y);
			if (entity && entity->type == TYPE_PEASANT && entity->team == team && entity->command && entity->command->type == COMMAND_BUILD)
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
	int peasants = map_count(map, TYPE_PEASANT, team);

	// build farms
	if (team->gold >= 80 && team->wood >= 40 && team->stone >= 20 && team->food == peasants && ai_buildpeasant(map, team) == NULL)
	{
		long anypoint = map_find(map, TYPE_PEASANT, team, point_create(0, 0));
		struct entity* anypeasant = map_get(map, point_getx(anypoint), point_gety(anypoint));
		if (anypeasant != NULL)
		{
			if (anypeasant->command)
			{
				command_destroy(anypeasant->command);
				anypeasant->command = NULL;
			}

			long castlepoint = map_find(map, TYPE_CASTLE, team, point_create(0, 0));
			if (castlepoint > -1)
			{
				int castlex = point_getx(castlepoint);
				int castley = point_gety(castlepoint);
				
				long searchpoint = point_create(castlex + 5, castley);
				long emptypoint = map_find(map, -1, NULL, searchpoint);
				anypeasant->command = (struct command*) build_create(emptypoint);
			}
		}
	}

	// collect resources
	struct entity* idlepeasant = ai_idlepeasant(map, team);
	if (idlepeasant != NULL)
	{
		int resourcetype;
		if (team->gold == 0)
		{
			resourcetype = TYPE_MINE;
		}		
		else if (team->wood == 0)
		{
			resourcetype = TYPE_TREE;
		}
		else if (team->stone == 0)
		{
			resourcetype = TYPE_ROCK;
		}
		else
		{
			resourcetype = ai_randomresourcetype();
		}
		
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
		return;
	}
	
	// train peasants
	if (team->gold >= 60 && team->food >= peasants)
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
		return;
	}
}
