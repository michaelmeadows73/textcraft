#ifndef ENTITY_H
#define ENTITY_H

#define TYPE_ROCK    	0
#define TYPE_TREE    	1
#define TYPE_PEASANT	2
#define TYPE_CASTLE	3
#define TYPE_MINE	4

#define SYMBOL_ROCK	'#'
#define SYMBOL_TREE	'T'
#define SYMBOL_PEASANT 	'P'
#define SYMBOL_CASTLE	'C'
#define SYMBOL_MINE	'M'

typedef void (*executefn)();

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
};

struct entity* entity_create();

void entity_destroy(struct entity*);

#endif
