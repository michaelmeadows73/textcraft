#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "list.h"
#include "point.h"
#include "map.h"
#include "entity.h"
#include "peasant.h"
#include "move.h"
#include "getwood.h"

void map_blockset(map, blockx, blocky, blockwidth, blockheight, type, symbol)
struct map* map;
int blockx;
int blocky;
int blockwidth;
int blockheight;
int type;
char symbol;
{
	int x, y;
	for (y = blocky; y < blocky + blockheight; y++)
	{
		for (x = blockx; x < blockx + blockwidth; x++)
		{
			struct entity* entity = entity_create(type, symbol);
			entity->point = point_create(x, y);
			map_set(map, x, y, entity);
		}
	}  	
}

void world_generate(map, entities)
struct map* map;
struct list* entities;
{
	srand(time(NULL));

	int i;
	for (i = 0; i < 10; i++)
	{
		struct entity* peasant = peasant_create(point_create(rand() % map->width, rand() % map->height));
		list_add(entities, peasant);
	}

	int j;
	for (j = 0; j < 20; j++)
	{
		map_blockset(map, rand() % (map->width - 10), rand() % (map->height - 10), rand() % 10, rand() % 10, TYPE_TREE, SYMBOL_TREE);
		map_blockset(map, rand() % (map->width - 10), rand() % (map->height - 10), rand() % 10, rand() % 10, TYPE_ROCK, SYMBOL_ROCK);
	}

	struct entity* castle = entity_create(TYPE_CASTLE, SYMBOL_CASTLE);
	int cx = rand() % map->width;
	int cy = rand() % map->height;
	castle->point = point_create(cx, cy);
	map_set(map, rand() % map->width, rand() % map->height, castle); 
}

void entity_execute(entity, map)
struct entity* entity;
struct map* map;
{
	entity->execute(entity, map);
}

int entity_samepoint(entity, point)
struct entity* entity;
long point;
{
	return entity->point == point;
}

main()
{
	int i = 0;
	int j = 0;

	int x = 0;
	int y = 0;

	initscr();
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);

	int width;
	int height;
	getmaxyx(stdscr, height, width);

	struct map* map = map_create(width, height);
	struct list* entities = list_create();

	world_generate(map, entities);

	int cx = 1;
	int cy = 1;	
	//map_set(map, cx, cy, -map_get(map, cx, cy));

	int clock = 0;
	int running = 1;
	while (running)
	{
		//map_set(map, cx, cy, -map_get(map, cx, cy));

		timeout(1);

		char c;
		long target;
		struct entity* mapentity;
		switch (c = getch())
		{
			case 'w':
			case 'W':
				cy--;
				break;
			case 's':
			case 'S':
				cy++;
				break;
			case 'a':
			case 'A':
				cx--;
				break;
			case 'd':
			case 'D':
				cx++;
				break;
			case ' ':
				mapentity = map_get(map, cx, cy);
				if (mapentity)
				{
					mapentity->selected = 1;
				}	
				break;
			case 'l':
			case 'L':
				target = point_create(cx, cy);
				mapentity = map_get(map, cx, cy);
				for (y = 0; y < map->height; y++)
				{
					for (x = 0; x < map->width; x++)
					{
						struct entity* entity = map_get(map, x, y);
						if (entity && entity->selected)
						{
							if (entity->command == NULL)
							{
								if (mapentity && mapentity->type == TYPE_TREE)
								{
									entity->command = getwood_create(target);
			
								}
								if (mapentity == NULL)
								{
									entity->command = move_create(target);
								
								}
							}
						}
					}
				}
				break;
			case 27:
				for (y = 0; y < map->height; y++)
				{
					for (x = 0; x < map->width; x++)
					{
						struct entity* entity = map_get(map, x, y);
						if (entity && entity->selected)
						{
							entity->selected = 0;
						}
					}
				}
				break;
			case 'q':
			case 'Q':
				running = 0;
				break;
		}

		if (clock++ % 40 == 0)
		{
			list_iterate(entities, entity_execute, map);	
		}
		
		map_print(map, cx, cy);

		usleep(100);
	}

	list_iterate(entities, peasant_destroy, NULL);
	list_destroy(entities);
	// TODO destroy all map entities properly
	map_destroy(map);

	endwin();
}
