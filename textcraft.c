#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "point.h"
#include "list.h"
#include "map.h"
#include "entity.h"
#include "peasant.h"

struct list* map_shortestpath(map, start, finish)
struct map* map;
long start;
long finish;
{
	struct list* start_path = list_create();
	list_add(start_path, (void*) start);

	struct list* queue = list_create();
	list_add(queue, start_path);

	struct list* visited = list_create();
	list_add(visited, (void*) start);

	while (!list_empty(queue))
	{
		struct list* current_path = list_getfirst(queue);
		list_removefirst(queue);
		
		long current = (long) list_getlast(current_path);	
		if (point_equals(current, finish))
		{
			list_iterate(queue, list_destroy, NULL);
			list_destroy(queue);
			list_destroy(visited);
			return current_path;
		}

		int cx = point_getx(current);
		int cy = point_gety(current);
		
		int pass;
		for (pass = 0; pass <=1 ; pass++)
		{
			int dx, dy;
			for (dy = -1; dy <= +1; dy++)
			{
				for (dx = -1; dx <= +1; dx++)
				{
					// first pass (0) consider non-diagonal neighbours
					// second pass (1) consider diagonal neighbours
					if ((!pass && ((dx && !dy) || (!dx && dy))) || (pass && dx && dy))
					{
						int x = cx + dx;
						int y = cy + dy;
						if (x >= 0 && x < map->width && y >= 0 && y < map->height && map_get(map, x, y) == ' ')
						{
							long neighbour = point_create(x, y);
							
							if (!list_contains(visited, (void*) neighbour, point_equals))
							{
								struct list* neighbour_path = list_clone(current_path);
								list_add(neighbour_path, (void*) neighbour);
								list_add(queue, neighbour_path);
								list_add(visited, (void*) neighbour);
							}
						}
					}
				}
			}
		}

		list_destroy(current_path);
	}

	list_iterate(queue, list_destroy, NULL);
	list_destroy(queue);
	list_destroy(visited);
	return NULL;	
}

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

	struct entity* selected = NULL;

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
				if (selected)
				{
					selected->path = map_shortestpath(map, selected->point, point_create(cx, cy));
				}
				else
				{
					selected = list_find(entities, entity_samepoint, (void*) point_create(cx, cy));		
				}
				break;
			case 27:
				selected = NULL;
				break;
			case 'q':
			case 'Q':
				running = 0;
				break;
		}

		list_iterate(entities, entity_execute, map);	
	
		if (selected)
		{
			int sx = point_getx(selected->point);
			int sy = point_gety(selected->point);
			map_set(map, sx, sy, -map_get(map, sx, sy)); 		
		}	

		map_set(map, cx, cy, -map_get(map, cx, cy));
		
		map_print(map);
		usleep(100000);
	}

	list_iterate(entities, peasant_destroy, NULL);
	list_destroy(entities);
	map_destroy(map);

	endwin();
}
