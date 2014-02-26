#include <stdlib.h>
#include "point.h"
#include "list.h"
#include "map.h"
#include "entity.h"
#include "peasant.h"

void peasant_execute(peasant, map)
struct entity* peasant;
struct map* map;
{
	if (peasant->path)
	{
		map_set(map, point_getx(peasant->point), point_gety(peasant->point), ' ');
		
		if (!list_empty(peasant->path))
		{
			peasant->point = (long) list_getfirst(peasant->path);
			list_removefirst(peasant->path);
		}
		else
		{
			list_destroy(peasant->path);
			peasant->path = NULL;
		}
	}

	map_set(map, point_getx(peasant->point), point_gety(peasant->point), 'P');
}

struct entity* peasant_create(point)
long point;
{
	struct entity* peasant = entity_create();
	peasant->point = point;
	peasant->path = NULL;
	peasant->execute = peasant_execute;
	return peasant;
}

void peasant_destroy(peasant)
struct entity* peasant;
{
	entity_destroy(peasant);
} 
