typedef void (*executefn)();

struct entity
{
	long point;
	struct list* path;
	executefn execute;
};

struct entity* entity_create();

void entity_destroy(struct entity*);
