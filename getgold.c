#include "team.h"
#include "gather.h"

void getgold_collect(team)
struct team* team;
{
	team->gold += 10;
}

struct gather* getgold_create(target)
long target;
{
	
	return gather_create(target, TYPE_MINE, getgold_collect, "Mining Gold");
}
