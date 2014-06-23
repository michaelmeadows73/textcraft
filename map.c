#include <stdlib.h>
#include <limits.h>
#include <ncurses.h>
#include "point.h"
#include "list.h"
#include "entity.h"
#include "command.h"
#include "team.h"
#include "map.h"

void map_clear(map)
struct map* map;
{
 	struct entity** current = map->entities;
        struct entity** finish = map->entities + (map->width * map->height);
	
	while (current < finish)
	{
		*(current++) = NULL;
	}
}

struct map* map_create(width, height)
int width;
int height;
{
	struct map* map = (struct map*) malloc(sizeof(struct map));
	map->width = width;
	map->height = height;
	map->entities = (struct entity**) malloc(sizeof(struct entity*) * width * height);

	map_clear(map);

	return map; 
}

void map_destroy(map)
struct map* map;
{
	struct entity** current = map->entities;
	struct entity** finish = map->entities + (map->width * map->height);

	while (current < finish)
	{
		struct entity* entity = *current;
		if (entity)
		{
			entity_destroy(entity);
		}
		*(current++) = NULL;
	}

	free(map);
}

struct entity* map_get(map, x, y)
struct map* map;
int x;
int y;
{
	struct entity** p = map->entities + x + y * map->width;
	
	return *p;
}

void map_set(map, x, y, entity)
struct map* map;
int x;
int y;
struct entity* entity;
{
	struct entity** p = map->entities + x + y * map->width;

	*p = entity;
}

void map_print(map, cx, cy)
struct map* map;
int cx;
int cy;
{
	int x, y;
	for (y = 0; y < map->height; y++) 
	{
		for (x = 0; x < map->width; x++)
		{
			struct entity* entity = map_get(map, x, y);
			int selected = 0;
			char symbol = ' ';

			if (entity)
			{
				selected = entity->selected;
				symbol = entity->symbol;
			}

			if (cx == x && cy == y)
			{
				selected = selected ? 0 : 1;
			}

			int color = (selected ? 2 : 1);
			if (entity && entity->team)
			{
				color += 2 * entity->team->id;
			}
	
			attron(COLOR_PAIR(color));
			mvprintw(y, x, "%c", symbol);
			attroff(COLOR_PAIR(color));
		}
	}
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

		                                        if (map_get(map, x, y))
							{
								if (point_equals(neighbour, finish))
								{
									list_iterate(queue, list_destroy, NULL);
									list_destroy(queue);
									list_destroy(visited);
									return current_path;
								}
							}
							else
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

long map_find(map, type, team, start)
struct map* map;
int type;
struct team* team;
long start;
{
	int x0 = point_getx(start);
	int y0 = point_gety(start);

	long mind = LONG_MAX;
	long minpoint = -1;

	int x1;
	int y1;
	struct entity** current = map->entities;
	for (y1 = 0; y1 < map->height; y1++)
	{
		for (x1 = 0; x1 < map->width; x1++)
		{
			struct entity* entity = *(current++);
			if (entity && entity->type == type && entity->team == team)
			{
				long d = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
				if (d < mind)
				{
					mind = d;
					minpoint = point_create(x1, y1);
				}				
			}
		}
	}

	return minpoint;
}

void map_execute_entity(entity, map)
struct entity* entity;
struct map* map;
{
	entity->execute(entity, map);

	if (entity->health <= 0)
	{
		map_set(map, point_getx(entity->point), point_gety(entity->point), NULL);
		entity_destroy(entity);
	}
}

void map_execute(map)
struct map* map;
{
	struct list* entities = list_create();

	struct entity** current = map->entities;
        struct entity** finish = map->entities + (map->width * map->height);
	
	while (current < finish)
	{
		struct entity* entity = *current;
		if (entity && entity->execute)
		{
			list_add(entities, entity); 
		}
		current++;
	}

	list_iterate(entities, map_execute_entity, map);
	list_destroy(entities);
}

int map_count(map, type, team)
struct map* map;
int type;
struct team* team;
{
	int count = 0;

	struct entity** current = map->entities;
        struct entity** finish = map->entities + (map->width * map->height);
	
	while (current < finish)
	{
		struct entity* entity = *current;
		if (entity && entity->type == type && entity->team == team)
		{
			count++; 
		}
		current++;
	}

	return count;
}
