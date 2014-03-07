#include <stdlib.h>
#include <ncurses.h>
#include "point.h"
#include "map.h"

void map_clear(map)
struct map* map;
{
	char* current = map->data;
	char* finish = map->data + (map->width * map->height);
	
	while (current < finish)
	{
		*(current++) = ' ';
	}
}

struct map* map_create(width, height)
int width;
int height;
{
	struct map* map = (struct map*) malloc(sizeof(struct map));
	map->width = width;
	map->height = height;
	map->data = (char*) malloc(sizeof(char) * width * height);

	map_clear(map);

	return map; 
}

void map_destroy(map)
struct map* map;
{
	free(map);
}

char map_get(map, x, y)
struct map* map;
int x;
int y;
{
	char* p = map->data + x + y * map->width;
	
	return *p;
}

void map_set(map, x, y, value)
struct map* map;
int x;
int y;
char value;
{
	char* p = map->data + x + y * map->width;

	*p = value;
}

void map_print(map)
struct map* map;
{
	int x, y;

	for (y = 0; y < map->height; y++) 
	{
		for (x = 0; x < map->width; x++)
		{
			char c = map_get(map, x, y);
			attron(COLOR_PAIR(c > 0 ? 1 : 2));
			mvprintw(y, x, "%c", c > 0 ? c : -c);
			attroff(COLOR_PAIR(c > 0 ? 1 : 2));
		}
	}
	refresh();
}

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
						if (x >= 0 && x < map->width && y >= 0 && y < map->height)
						{
							long neighbour = point_create(x, y);
		                                        if (map_get(map, x, y) != ' ' && point_equals(neighbour, finish))
							{
								list_iterate(queue, list_destroy, NULL);
								list_destroy(queue);
								list_destroy(visited);
								return current_path;
								
							}				
							
							if (map_get(map, x, y) == ' ')
							{
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
		}

		list_destroy(current_path);
	}

	list_iterate(queue, list_destroy, NULL);
	list_destroy(queue);
	list_destroy(visited);
	return NULL;	
}
