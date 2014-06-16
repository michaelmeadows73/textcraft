#include <stdlib.h>
#include <stdio.h>
#include "point.h"
#include "team.h"
#include "map.h"
#include "command.h"
#include "entity.h"
#include "peasant.h"

void peasant_execute(peasant, map)
struct entity* peasant;
struct map* map;
{
	map_set(map, point_getx(peasant->point), point_gety(peasant->point), NULL);
	
	if (peasant->command)
	{
		int finished = peasant->command->execute(peasant->command, peasant, map);
		if (finished)
		{
			command_destroy(peasant->command);
			peasant->command = NULL;
		}
	}
	
	map_set(map, point_getx(peasant->point), point_gety(peasant->point), peasant);
}

void peasant_tostring(peasant, str)
struct entity* peasant;
char* str;
{
	if (peasant->command == NULL || peasant->command->desc == NULL)
		sprintf(str, "Peasant: %d (Idling)", peasant->health);
	else
		sprintf(str, "Peasant: %d (%s)", peasant->health, peasant->command->desc);
}

struct entity* peasant_create(point)
long point;
{
	struct entity* peasant = entity_create(TYPE_PEASANT, SYMBOL_PEASANT);
	peasant->point = point;
	peasant->health = 100;
	peasant->command = NULL;
	peasant->execute = peasant_execute;
	peasant->tostring = peasant_tostring;
	return peasant;
}
