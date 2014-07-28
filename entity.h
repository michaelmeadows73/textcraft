#ifndef ENTITY_H
#define ENTITY_H

#define TYPE_NONE	0
#define TYPE_ROCK    	1
#define TYPE_TREE    	2
#define TYPE_PEASANT	3
#define TYPE_CASTLE	4
#define TYPE_MINE	5
#define TYPE_FARM       6

#define SYMBOL_ROCK	'#'
#define SYMBOL_TREE	'T'
#define SYMBOL_PEASANT 	'P'
#define SYMBOL_CASTLE	'C'
#define SYMBOL_MINE	'M'
#define SYMBOL_FARM     'F'

typedef void (*executefn)();  // struct entity*, struct map*
typedef void (*tostringfn)(); // struct entity*, char*

struct entity
{
	int id;
	int type;
	char symbol;
	struct team* team;
	long point;
	int health;
	int selected;
	struct command* command;
	
	executefn execute;
	tostringfn tostring;
};

struct entity* entity_create(int, char);

void entity_destroy(struct entity*);

#endif
