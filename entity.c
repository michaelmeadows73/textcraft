#include <stdlib.h>
#include "entity.h"

struct entity* entity_create()
{
	struct entity* entity = (struct entity*) malloc(sizeof(struct entity));
	entity->point = 0;
	entity->path = NULL;
	entity->execute = NULL;
	return entity;
}

void entity_destroy(entity)
struct entity* entity;
{
	free(entity);
}
