#ifndef FLEE_H
#define FLEE_H

#include "command.h"
#include "entity.h"
#include "map.h"

int flee_execute(struct command*, struct entity*, struct map*);

struct command* flee_create();

#endif
