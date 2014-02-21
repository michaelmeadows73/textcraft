struct point
{
	int x;
	int y;
};

struct point* point_create(int, int);

int point_equals(struct point*, struct point*);
