
#define NDEBUG 1

#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <ncurses.h>
#include <assert.h>
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

struct node
{
	long point;
	double f;
	double g;
	double h;
	struct node* parent;
};

struct node* node_create()
{
	struct node* node = (struct node*) malloc(sizeof(struct node));
	node->point = 0;
	node->f = 0.0;
	node->g = 0.0;
	node->h = 0.0;
	node->parent = NULL;
	return node;
}

void node_destroy(node)
struct node* node;
{
	free(node);
}

int node_equals(node0, node1)
struct node* node0;
struct node* node1;
{
	return point_equals(node0->point, node1->point);
}

void nodes_destroy(nodes_start, nodes_finish)
struct node** nodes_start;
struct node** nodes_finish;
{
	struct node** p = nodes_start;
	while (p < nodes_finish)
	{
		struct node* n = *p;
		if (n)
		{
			node_destroy(n);
		}
		p++;
	}
	free(nodes_start);
}

struct node* nodes_minimum(nodes_start, nodes_finish)
struct node** nodes_start;
struct node** nodes_finish;
{
	struct node* m = NULL;
	struct node** p = nodes_start;
	while (p < nodes_finish)
	{
		struct node* n = *p;
		if (n)
		{
			if (!m || n->f < m->f)
			{
				m = n;
			}
		} 
		p++;
	}
	return m;
}

void node_add(map, start, finish, open, closed, current_node, nx, ny)
struct map* map;
long start;
long finish;
struct node** open;
struct node** closed;
struct node* current_node;
int nx;
int ny;
{
	if (nx >= 0 && nx < map->width && ny >= 0 && ny < map->height)
	{
		struct entity* entity = map_get(map, nx, ny);
		if ((map_get(map, nx, ny) == NULL || point_create(nx, ny) == finish) && !closed[nx + ny * map->width])
		{	
			struct node* neighbour_node = node_create();
			neighbour_node->point = point_create(nx, ny);
			neighbour_node->g = current_node->g + point_dist(current_node->point, neighbour_node->point);
			neighbour_node->h = point_dist(neighbour_node->point, finish);
			neighbour_node->f = neighbour_node->g + neighbour_node->h;
			neighbour_node->parent = current_node;

			struct node* open_node = open[nx + map->width * ny];
			if (open_node)
			{
				if (open_node->f > neighbour_node->f)
				{
					open_node->g = neighbour_node->g;
					open_node->h = neighbour_node->h;
					open_node->f = open_node->g + open_node->h;
					open_node->parent = neighbour_node->parent;
	
				}
			}
			else
			{
				open[nx + map->width * ny] = neighbour_node;
			}
		}
	}
}

struct list* map_shortestpath(map, start, finish)
struct map* map;
long start;
long finish;
{
	struct node** open = (struct node **) calloc(map->width * map->height, sizeof(struct node*));
	struct node** closed = (struct node **) calloc(map->width * map->height, sizeof(struct node*)); 

	struct node** open_finish = open + map->width * map->height;
	struct node** closed_finish = closed + map->width * map->height;

	struct node* start_node = node_create();
	start_node->point = start;
	start_node->g = 0.0;
	start_node->h = point_dist(start, finish); 
	start_node->f = start_node->g + start_node->h;
	start_node->parent = NULL;

	int sx = point_getx(start);
	int sy = point_gety(start);
	open[sx + map->width * sy] = start_node;

	while (1)
	{
		struct node* current_node = nodes_minimum(open, open_finish);
		if (current_node == NULL)
		{
			break;
		}

		int cx = point_getx(current_node->point);
		int cy = point_gety(current_node->point);
		
		open[cx + map->width * cy] = NULL;		
		closed[cx + map->width * cy] = current_node;
	
		if (point_equals(current_node->point, finish))
		{
			struct list* current_path = list_create();
			while (current_node)
			{
				list_prepend(current_path, (void*) current_node->point);
				current_node = current_node->parent;
			}

			nodes_destroy(open, open_finish);
			nodes_destroy(closed, closed_finish);
			return current_path;
		}
		
		int dx, dy;
		for (dy = -1; dy <= +1; dy++)
		{
			for (dx = -1; dx <= +1; dx++)
			{
				if (dx || dy)
				{
					int nx = cx + dx;
					int ny = cy + dy;
					
					node_add(map, start, finish, open, closed, current_node, nx, ny);
				}
			}
		}
	}

	nodes_destroy(open, open_finish);
	nodes_destroy(closed, closed_finish);
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
			if (entity && (entity->type == type || type == TYPE_NONE) && entity->team == team)
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

int map_hasunits(map, team)
struct map* map;
struct team* team;
{
	struct entity** current = map->entities;
	struct entity** finish = map->entities + (map->width * map->height);

	while (current < finish)
	{
		struct entity* entity = *current;
		if (entity && entity->team == team)
		{
			return 1;
		}
		current++;
	}
	return 0;
}
