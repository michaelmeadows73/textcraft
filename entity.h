#define TYPE_ROCK    	0
#define TYPE_TREE    	1
#define TYPE_PEASANT	2
#define TYPE_CASTLE	3

#define SYMBOL_ROCK	'#'
#define SYMBOL_TREE	'T'
#define SYMBOL_PEASANT 	'P'
#define SYMBOL_CASTLE	'C'

typedef void (*executefn)();
typedef void (*destroyfn)();

struct entity
{
	int type;
	char symbol;
	int team;
	long point;
	int selected;
	struct command* command;
	
	executefn execute;
	destroyfn destroy;
};

struct entity* entity_create();

void entity_destroy(struct entity*);
