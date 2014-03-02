#include <stdlib.h>
#include "point.h"
#include "map.h"
#include "entity.h"
#include "peasant.h"

void peasant_execute(peasant, map)
struct entity* peasant;
struct map* map;
{
	if (peasant->target >= 0 && peasant->path == NULL)
	{
		peasant->path = map_shortestpath(map, peasant->point, peasant->target);
		if (peasant->path == NULL)
		{
			peasant->target = -1;
		}
	}

	if (peasant->path)
	{
		map_set(map, point_getx(peasant->point), point_gety(peasant->point), ' ');
		
		if (!list_empty(peasant->path))
		{
			long nextpoint = (long) list_getfirst(peasant->path);
			if (map_get(map, point_getx(nextpoint), point_gety(nextpoint)) == ' ')
			{
				peasant->point = nextpoint;
				list_removefirst(peasant->path);
			}
			else
			{
				list_destroy(peasant->path);
				peasant->path = NULL;
			}
		}
		else
		{
			list_destroy(peasant->path);
			peasant->path = NULL;
			peasant->target = -1;
		}
	}

	map_set(map, point_getx(peasant->point), point_gety(peasant->point), 'P');
}

struct entity* peasant_create(point)
long point;
{
	struct entity* peasant = entity_create();
	peasant->point = point;
	peasant->target = -1;
	peasant->path = NULL;
	peasant->execute = peasant_execute;
	return peasant;
}

void peasant_destroy(peasant)
struct entity* peasant;
{
	entity_destroy(peasant);
} 
