#include "team.h"
#include "gather.h"

void getstone_collect(team)
struct team* team;
{
	team->stone += 10;
}

struct gather* getstone_create(target)
long target;
{
	return gather_create(target, TYPE_ROCK, 1, getstone_collect);
}
