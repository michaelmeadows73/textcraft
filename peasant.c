#include <stdlib.h>
#include "point.h"
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

struct entity* peasant_create(point)
long point;
{
	struct entity* peasant = entity_create(TYPE_PEASANT, SYMBOL_PEASANT);
	peasant->point = point;
	peasant->command = NULL;
	peasant->execute = peasant_execute;
	peasant->destroy = peasant_destroy;
	return peasant;
}

void peasant_destroy(peasant)
struct entity* peasant;
{
	entity_destroy(peasant);
}
