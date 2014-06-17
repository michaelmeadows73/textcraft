#include <stdlib.h>
#include <stdio.h>
#include "point.h"
#include "team.h"
#include "map.h"
#include "command.h"
#include "entity.h"
#include "castle.h"

void castle_execute(castle, map)
struct entity* castle;
struct map* map;
{
	if (castle->command)
	{
		int finished = castle->command->execute(castle->command, castle, map);
		if (finished)
		{
			command_destroy(castle->command);
			castle->command = NULL;
		}
	}
}

void castle_tostring(castle, str)
struct entity* castle;
char* str;
{
	if (castle->command == NULL || castle->command->desc == NULL)
		sprintf(str, "Castle: %d (Quiet)", castle->health);
	else
		sprintf(str, "Castle: %d (%s)", castle->health, castle->command->desc);
}

struct entity* castle_create(point)
long point;
{
	struct entity* castle = entity_create(TYPE_CASTLE, SYMBOL_CASTLE);
	castle->point = point;
	castle->health = 10000;
	castle->command = NULL;
	castle->execute = castle_execute;
	castle->tostring = castle_tostring;
	return castle;
}
