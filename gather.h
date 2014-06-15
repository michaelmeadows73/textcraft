#ifndef GATHER_H
#define GATHER_H

#include "command.h"

struct command* gather_create(long, int, int, collectfn);

#endif
