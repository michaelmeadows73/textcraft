typedef void (*executefn)();

struct entity
{
	long point;
	long target;
	struct list* path;
	executefn execute;
};

struct entity* entity_create();

void entity_destroy(struct entity*);
