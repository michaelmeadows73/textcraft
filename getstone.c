#include "team.h"
#include "command.h"
#include "gather.h"

void getstone_collect(team)
struct team* team;
{
	team->stone += 10;
}

struct command* getstone_create(target)
long target;
{
	return gather_create(target, TYPE_ROCK, 1, getstone_collect);
}
