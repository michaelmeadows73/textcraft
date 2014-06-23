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

void team_destroy(team)
struct team* team;
{
	free(team);
}
