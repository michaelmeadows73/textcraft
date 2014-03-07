typedef void (*executefn)();

struct entity
{
	long point;
	struct command* command;
	
	executefn execute;
};

struct entity* entity_create();

void entity_destroy(struct entity*);
