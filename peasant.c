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
	map_set(map, point_getx(peasant->point), point_gety(peasant->point), ' ');
	
	if (peasant->command)
	{
		int finished = peasant->command->execute(peasant->command, peasant, map);
		if (finished)
		{
			command_destroy(peasant->command);
			peasant->command = NULL;
		}
	}
	
	map_set(map, point_getx(peasant->point), point_gety(peasant->point), 'P');
}

struct entity* peasant_create(point)
long point;
{
	struct entity* peasant = entity_create();
	peasant->point = point;
	peasant->command = NULL;
	peasant->execute = peasant_execute;
	return peasant;
}

void peasant_destroy(peasant)
struct entity* peasant;
{
	entity_destroy(peasant);
} 
