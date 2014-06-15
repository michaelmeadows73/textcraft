#include "team.h"
#include "command.h"
#include "gather.h"

void getwood_collect(team)
struct team* team;
{
	team->wood += 10;
}

struct command* getwood_create(target)
long target;
{
	
	return gather_create(target, TYPE_TREE, 1, getwood_collect);
}
