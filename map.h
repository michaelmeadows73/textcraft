struct map 
{
	int width;
	int height;
	struct entity** entities;
};

void map_clear(struct map*);

struct map* map_create(int, int);

void map_destroy(struct map*);

struct entity* map_get(struct map*, int, int);

void map_set(struct map*, int, int, struct entity*);

void map_print(struct map*, int, int);

struct list* map_shortestpath(struct map*, long, long);

long map_find(struct map*, int, long);
