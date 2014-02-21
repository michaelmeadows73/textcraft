#include <stdlib.h>
#include <ncurses.h>
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
			attron(COLOR_PAIR(1));
			mvprintw(y, x, "%c", c);
			attroff(COLOR_PAIR(1));
		}
	}
	refresh();
}
