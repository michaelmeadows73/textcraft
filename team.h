struct team
{
	int id;
	int gold;
	int wood;
	int stone;
};

struct team* team_create(int);

void team_print(struct team*);

void team_destroy(struct team*);
