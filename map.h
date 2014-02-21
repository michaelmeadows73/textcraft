struct map 
{
	int width;
	int height;
	char* data;
};

void map_clear(struct map*);

struct map* map_create(int, int);

void map_destroy(struct map*);

char map_get(struct map*, int, int);

void map_set(struct map*, int, int, char);

void map_print(struct map*);
