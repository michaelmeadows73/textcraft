#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "point.h"
#include "list.h"
#include "map.h"

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

main()
{
	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);

	int width;
	int height;
	getmaxyx(stdscr, height, width);

	struct map* map = map_create(width, height);

	int px = 5;
	int py = 4;
	int sx = 1;
	int sy = 1;	
	map_set(map, px, py, 'P');
	map_set(map, sx, sy, '*');
	map_set(map, 14, 12, 'T');

	int y;
	for (y = 3; y < 17; y++)
	{
		map_set(map, 10, y, '#');
	}

	int x;
	for (x = 3; x < 17; x++)
	{
		map_set(map, x, 8, '#');
	}

	struct list* shortestpath = NULL;
	long start = 0;
	long finish = 0;

	int running = 1;
	while (running)
	{
		map_set(map, sx, sy, ' ');

		timeout(1);
		switch (getch())
		{
			case 'w':
			case 'W':
				sy--;
				break;
			case 's':
			case 'S':
				sy++;
				break;
			case 'a':
			case 'A':
				sx--;
				break;
			case 'd':
			case 'D':
				sx++;
				break;
			case ' ':
				start = point_create(px, py);
				finish = point_create(sx - 1, sy);
				shortestpath = map_shortestpath(map, start, finish);
				break;
			case 'q':
			case 'Q':
				running = 0;
				break;
		}

		map_set(map, sx, sy, '*');
		
		if (shortestpath)
		{
			if (!list_empty(shortestpath))
			{
				long point = (long) list_getfirst(shortestpath);
				list_removefirst(shortestpath);
		
				map_set(map, px, py, ' ');
				px = point_getx(point);
				py = point_gety(point);
				map_set(map, px, py, 'P');
			}
			else
			{
				list_destroy(shortestpath);
				shortestpath = NULL;
			}
		}
		
		map_print(map);
		usleep(100000);
	}

	map_destroy(map);

	endwin();
}
