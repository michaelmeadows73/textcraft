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

void map_blockset(map, blockx, blocky, blockwidth, blockheight, type, symbol, health)
struct map* map;
int blockx;
int blocky;
int blockwidth;
int blockheight;
int type;
char symbol;
int health;
{
	int x, y;
	for (y = blocky; y < blocky + blockheight; y++)
	{
		for (x = blockx; x < blockx + blockwidth; x++)
		{
			struct entity* entity = entity_create(type, symbol);
			entity->point = point_create(x, y);
			entity->health = health;
			map_set(map, x, y, entity);
		}
	}  	
}

void mine_tostring(mine, str)
struct entity* mine;
char* str;
{
	sprintf(str, "Mine: %d", mine->health);
}

void world_generate(map, team1, team2)
struct map* map;
struct team* team1;
struct team* team2;
{
	srand(time(NULL));

	int j;
	for (j = 0; j < 20; j++)
	{
		map_blockset(map, rand() % (map->width - 10), rand() % (map->height - 10), rand() % 10, rand() % 10, TYPE_TREE, SYMBOL_TREE, 10);
		map_blockset(map, rand() % (map->width - 10), rand() % (map->height - 10), rand() % 10, rand() % 10, TYPE_ROCK, SYMBOL_ROCK, 20);
	}

	int i;
	for (i = 0; i < 10; i++)
	{
		int px = rand() % map->width;
		int py = rand() % map->height;
		struct entity* peasant = peasant_create(point_create(px, py));
		peasant->team = i < 5 ? team1 : team2;
		map_set(map, px, py, peasant);
	}

	int k;
	for (k = 0; k < 5; k++)
	{	
		struct entity* mine = entity_create(TYPE_MINE, SYMBOL_MINE);
		int mx = rand() % map->width;
		int my = rand() % map->height;
		mine->point = point_create(mx, my);
		mine->health = 500 + 10 * (rand() % 250);
		mine->tostring = mine_tostring;
		map_set(map, mx, my, mine);
	}

	int teamid;
	for (teamid = 1; teamid <= 2; teamid++)
	{
		int cx = rand() % map->width;
		int cy = rand() % map->height;

		struct entity* castle = castle_create(point_create(cx, cy));
		castle->team = teamid == 1 ? team1 : team2;
		map_set(map, cx, cy, castle); 
	}
}
