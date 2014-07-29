#include "team.h"
#include "gather.h"

void getwood_collect(team)
struct team* team;
{
	team->wood += 10;
}

struct gather* getwood_create(target)
long target;
{
	
	return gather_create(target, TYPE_TREE, getwood_collect, "Collecting Wood");
}
