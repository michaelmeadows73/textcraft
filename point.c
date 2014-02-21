#include <stdlib.h>
#include "point.h"

struct point* point_create(x, y)
int x;
int y;
{
	// pointless comment
	struct point* point = (struct point*) malloc(sizeof(struct point));
	point->x = x;
	point->y = y;
	return point;
}

int point_equals(point0, point1)
struct point* point0;
struct point* point1;
{
	return ((point0->x == point1->x) && (point0->y == point1->y));
}
