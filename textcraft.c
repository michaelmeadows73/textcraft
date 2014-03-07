#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "point.h"
#include "map.h"
#include "entity.h"
#include "peasant.h"

void map_blockset(map, blockx, blocky, blockwidth, blockheight, value)
struct map* map;
int blockx;
int blocky;
int blockwidth;
int blockheight;
char value;
{
	int x, y;
	for (y = blocky; y < blocky + blockheight; y++)
	{
		for (x = blockx; x < blockx + blockwidth; x++)
		{
			map_set(map, x, y, value);
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
		map_blockset(map, rand() % (map->width - 10), rand() % (map->height - 10), rand() % 10, rand() % 10, 'T');
		map_blockset(map, rand() % (map->width - 10), rand() % (map->height - 10), rand() % 10, rand() % 10, '#');
	}
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
	struct entity* selectedentity = NULL;

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

	struct list* selectedentities = list_create();

	int cx = 1;
	int cy = 1;	
	map_set(map, cx, cy, -map_get(map, cx, cy));

	int running = 1;
	while (running)
	{
		map_set(map, cx, cy, -map_get(map, cx, cy));

		timeout(1);

		char c;
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
				selectedentity = list_find(entities, entity_samepoint, (void*) point_create(cx, cy));
				if (selectedentity)
				{
					list_add(selectedentities, selectedentity);
				}	
				break;
			case 'l':
			case 'L':
				for (i = 0; i < list_count(selectedentities); i++)
				{
					long target = point_create(cx, cy);
					selectedentity = (struct entity*) list_getitem(selectedentities, i);
					if (selectedentity->command == NULL)
					{
						selectedentity->command = move_create(target);
					}
				}
				break;
			case 27:
				list_clear(selectedentities);
				break;
			case 'q':
			case 'Q':
				running = 0;
				break;
		}

		list_iterate(entities, entity_execute, map);	

		for (j = 0; j < list_count(selectedentities); j++)
		{
			selectedentity = (struct entity*) list_getitem(selectedentities, j);
			int sx = point_getx(selectedentity->point);
			int sy = point_gety(selectedentity->point);
			map_set(map, sx, sy, -map_get(map, sx, sy)); 		
		}

		map_set(map, cx, cy, -map_get(map, cx, cy));
		
		map_print(map);
		usleep(100000);
	}

	list_iterate(entities, peasant_destroy, NULL);
	list_destroy(selectedentities);
	list_destroy(entities);
	map_destroy(map);

	endwin();
}
