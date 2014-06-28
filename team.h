#ifndef TEAM_H
#define TEAM_H

struct team
{
	int id;
	int gold;
	int wood;
	int stone;
	int food;
	struct team* enemy;

	char* message;
	int messagetime;
};

struct team* team_create(int);

void team_print(struct team*);

void team_destroy(struct team*);

char* team_getmessage(struct team*);

void team_setmessage(struct team*, char*);

#endif
