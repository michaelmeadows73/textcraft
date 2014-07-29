#ifndef GATHER_H
#define GATHER_H

#include "team.h"
#include "command.h"

typedef void (*collectfn)(struct team*);

struct gather
{
	struct command base;

	int collecttype;
	collectfn collect;
};

struct gather* gather_create(long, int, collectfn, char*);

#endif
