#include "team.h"
#include "command.h"
#include "gather.h"

void getgold_collect(team)
struct team* team;
{
	team->gold += 10;
}

struct command* getgold_create(target)
long target;
{
	
	return gather_create(target, TYPE_MINE, 0, getgold_collect);
}
