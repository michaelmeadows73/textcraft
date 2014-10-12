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
#include "generator.h"
#include "ai.h"

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

int map_select(map, cx, cy, type, team, minid, multiple)
struct map* map;
int cx;
int cy;
int type;
struct team* team;
int minid;
int multiple;
{
	if (!multiple)
	{
		map_clearselection(map);
	}

	struct entity* current = NULL;

	int x, y;
	for (y = 0; y < map->height; y++)
	{
		for (x = 0; x < map->width; x++)
		{
			struct entity* entity = map_get(map, x, y);
			if (entity && (entity->type == type) && (entity->team == team) && (entity->id >= minid))
			{
				if ((current == NULL) || (current && (entity->id < current->id)))
				{
					current = entity;
				}
			}
		}
	}

	if (current)
	{
		current->selected = 1;
	}
	
	return current ? current->id : -1;
}

void list_train_create(entity)
struct entity* entity;
{
	if (entity && entity->type == TYPE_CASTLE)
	{
		if (entity->command)
		{
			command_destroy(entity->command);
			entity->command = NULL;
		}
		entity->command = train_create();
	}
}

void list_build_create(entity, target)
struct entity* entity;
void* target;
{
	if (entity && entity->type == TYPE_PEASANT)
	{
		if (entity->command)
		{
			command_destroy(entity->command);
			entity->command = NULL;		
		}
		entity->command = build_create((long) target);
	}
}



struct team* map_getwinningteam(map, team0, team1)
struct map* map;
struct team* team0;
struct team* team1;
{
	if (!map_hasunits(map, team0))
	{
		return team1;
	}
	if (!map_hasunits(map, team1))
	{
		return team0;
	}
	return NULL;
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
				if (entity && entity->selected && (entity->team == playerteam || entity->team == NULL))
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
				if (mapentity && (mapentity->team == playerteam || mapentity->team == NULL))
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
						if (entity && entity->selected && (entity->team == playerteam) && (entity->type == TYPE_PEASANT))
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
			case 'j':
			case 'J':
				list_iterate(selection, list_train_create, NULL);
				break;
			case 'f':
			case 'F':
				list_iterate(selection, list_build_create, (void*) point_create(cx, cy));
				break;
			case 27:
				map_clearselection(map);
				break;
			case 'p':
			case 'P':
				selectid = map_select(map, cx, cy, TYPE_PEASANT, playerteam, selectid + 1, c == 'P');
				break;
			case 'c':
			case 'C':
				selectid = map_select(map, cx, cy, TYPE_CASTLE, playerteam, selectid + 1, 0);
				break;
			case 'm':
			case 'M':
				selectid = map_select(map, cx, cy, TYPE_MINE, NULL, selectid + 1, 0);
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

		char* message = team_getmessage(playerteam);
		if (message)
		{
			mvprintw(height - 1, width - strlen(message), "%s", message);
		}

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
		
		struct team* winningteam = map_getwinningteam(map, playerteam, computerteam);
		if (winningteam)
		{
			if (winningteam == playerteam)
			{
				attron(COLOR_PAIR(6));
				mvprintw((height / 2) - 1, (width / 2) - 4, "         ");
				mvprintw((height / 2),     (width / 2) - 4, " Victory ");
				mvprintw((height / 2) + 1, (width / 2) - 4, "         ");
				attroff(COLOR_PAIR(6));
			}
			if (winningteam == computerteam)
			{
				attron(COLOR_PAIR(4));
				mvprintw((height / 2) - 1, (width / 2) - 4, "        ");
				mvprintw((height / 2),     (width / 2) - 4, " Defeat ");
				mvprintw((height / 2) + 1, (width / 2) - 4, "        ");
				attroff(COLOR_PAIR(4));
			}
			running = 0;
		}

		refresh();
	
		usleep(100);
	}

	map_destroy(map);

	endwin();
}
