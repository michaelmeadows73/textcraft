#include <stdlib.h>
#include "entity.h"

int nextid = 0;

struct entity* entity_create(type, symbol)
int type;
char symbol;
{
	struct entity* entity = (struct entity*) malloc(sizeof(struct entity));
	entity->id = nextid++;
	entity->type = type;
	entity->symbol = symbol;
	entity->team = NULL;
	entity->selected = 0;
	entity->point = 0;
	entity->health = 0;
	entity->command = NULL;
	entity->execute = NULL;
	entity->destroy = entity_destroy;
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
