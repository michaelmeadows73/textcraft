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
};

struct team* team_create(int);

void team_print(struct team*);

void team_destroy(struct team*);

#endif
