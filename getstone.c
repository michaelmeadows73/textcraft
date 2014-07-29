#include "team.h"
#include "gather.h"

void getstone_collect(team)
struct team* team;
{
	team->stone += 5;
}

struct gather* getstone_create(target)
long target;
{
	return gather_create(target, TYPE_ROCK, getstone_collect, "Quarrying Stone");
}
