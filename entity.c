#include <stdlib.h>
#include "entity.h"

struct entity* entity_create()
{
	struct entity* entity = (struct entity*) malloc(sizeof(struct entity));
	entity->point = 0;
	entity->command = NULL;
	entity->execute = NULL;
	return entity;
}

void entity_destroy(entity)
struct entity* entity;
{
	if (entity->command)
	{
		command_destroy(entity->command);
		entity->command = NULL;
	}

	free(entity);
}
