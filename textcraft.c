#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "list.h"
#include "point.h"
#include "team.h"
#include "map.h"
#include "entity.h"
#include "peasant.h"
#include "move.h"
#include "getwood.h"
#include "getstone.h"
#include "getgold.h"
#include "flee.h"
#include "attack.h"

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

void world_generate(map, team1, team2)
struct map* map;
struct team* team1;
struct team* team2;
{
	srand(time(NULL));

	int j;
	for (j = 0; j < 20; j++)
	{
		map_blockset(map, rand() % (map->width - 10), rand() % (map->height - 10), rand() % 10, rand() % 10, TYPE_TREE, SYMBOL_TREE);
		map_blockset(map, rand() % (map->width - 10), rand() % (map->height - 10), rand() % 10, rand() % 10, TYPE_ROCK, SYMBOL_ROCK);
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
		map_set(map, mx, my, mine);
	}

	int teamid;
	for (teamid = 1; teamid <= 2; teamid++)
	{
		struct entity* castle = entity_create(TYPE_CASTLE, SYMBOL_CASTLE);
		int cx = rand() % map->width;
		int cy = rand() % map->height;
		castle->team = teamid == 1 ? team1 : team2;
		castle->point = point_create(cx, cy);
		map_set(map, cx, cy, castle); 
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

void map_clearselection(map)
struct map* map;
{
	int x, y;
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
}

int map_select(map, cx, cy, type, team, minid)
struct map* map;
int cx;
int cy;
int type;
struct team* team;
int minid;
{
	map_clearselection(map);

	struct entity* peasant = NULL;

	int x, y;
	for (y = 0; y < map->height; y++)
	{
		for (x = 0; x < map->width; x++)
		{
			struct entity* entity = map_get(map, x, y);
			if (entity && (entity->type == type) && (entity->team == team) && (entity->id >= minid))
			{
				if ((peasant == NULL) || (peasant && (entity->id < peasant->id)))
				{
					peasant = entity;
				}
			}
		}
	}

	if (peasant)
	{
		peasant->selected = 1;
	}
	
	return peasant ? peasant->id : -1;
}

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
	struct entity* idlepeasant = ai_idlepeasant(map, team);
	if (idlepeasant != NULL)
	{
		int resourcetype = ai_randomresourcetype();
		
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
	}
}

main()
{
	struct team* team1 = team_create(1);
	struct team* team2 = team_create(2);

	team1->enemy = team2;
	team2->enemy = team1;

	struct team* playerteam = team1;
	struct team* computerteam = team2;

	int selectid = -1;

	int i = 0;
	int j = 0;

	int x = 0;
	int y = 0;

	initscr();
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_BLACK, COLOR_RED);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);
	init_pair(6, COLOR_BLACK, COLOR_BLUE);
	curs_set(0);
	
	int width;
	int height;
	getmaxyx(stdscr, height, width);

	struct map* map = map_create(width, height);
	world_generate(map, team1, team2);

	int cx = 1;
	int cy = 1;	

	int clock = 0;
	int running = 1;
	while (running)
	{
		timeout(1);

		char c;
		long target;
		struct entity* mapentity;
		
		// get selected entities
		struct list* selection = list_create();
		for (y = 0; y < map->height; y++)
		{
			for (x = 0; x < map->width; x++)
			{
				struct entity* entity = map_get(map, x, y);
				if (entity && entity->selected && (entity->team == playerteam))
				{
					list_add(selection, entity);
				}
			}
		}
	

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
				if (mapentity && mapentity->team == playerteam)
				{
					mapentity->selected = 1;
				}	
				break;
			case 'l':
			case 'L':
			case 'k':
			case 'K':
				target = point_create(cx, cy);
				mapentity = map_get(map, cx, cy);
				for (y = 0; y < map->height; y++)
				{
					for (x = 0; x < map->width; x++)
					{
						struct entity* entity = map_get(map, x, y);
						if (entity && entity->selected && (entity->team == playerteam))
						{
							if (entity->command)
							{
								command_destroy(entity->command);
								entity->command = NULL;	
							}
							if (c == 'l' || c == 'L')
							{
								if (mapentity && mapentity->type == TYPE_TREE)
								{
									entity->command = (struct command*) getwood_create(target);
								}
								if (mapentity && mapentity->type == TYPE_ROCK)
								{
									entity->command = (struct command*) getstone_create(target);
								}
								if (mapentity && mapentity->type == TYPE_MINE)
								{
									entity->command = (struct command*) getgold_create(target);
								}
								if (mapentity == NULL)
								{
									entity->command = move_create(target);
								
								}
							}
							else
							{
								entity->command = attack_create(target);
							}
						}
					}
				}
				break;
			case 27:
				map_clearselection(map);
				break;
			case 'p':
			case 'P':
				selectid = map_select(map, cx, cy, TYPE_PEASANT, playerteam, selectid + 1);
				break;
			case 'q':
			case 'Q':
				running = 0;
				break;
		}

		if (clock % 1600 == 0)
		{
			ai(map, computerteam);
		}

		if (clock % 40 == 0)
		{
			map_execute(map);
		}

		clock++;
		
		map_print(map, cx, cy);

		team_print(playerteam);

		// print selected entity
		if (list_count(selection) == 1)
		{
			struct entity* selected = list_getfirst(selection);
			if (selected->tostring)
			{
				char str[255];
				selected->tostring(selected, str);
				mvprintw(height - 1, 0, "%s", str);
			}
		}

		list_destroy(selection);

		refresh();
		
		usleep(100);
	}

	map_destroy(map);

	endwin();
}
