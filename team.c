#include <stdlib.h>
#include "team.h"

struct team* team_create(id)
int id;
{
	struct team* team = (struct team*) malloc(sizeof(struct team));
	team->id = id;
	team->gold = 0;
	team->wood = 0;
	team->stone = 0;
	team->food = 6;
	return team;
}

void team_print(team)
struct team* team;
{
	mvprintw(0, 0, "Gold: %d    Wood: %d    Stone: %d    Food: %d", team->gold, team->wood, team->stone, team->food);
}

char* team_getmessage(team)
struct team* team;
{
	if (team->message && team->messagetime-- == 0)
	{
		team->message = NULL;
	}
	return team->message;
}

void team_setmessage(team, message)
struct team* team;
char* message;
{
	team->message = message;
	team->messagetime = 1000;
}

void team_destroy(team)
struct team* team;
{
	free(team);
}
