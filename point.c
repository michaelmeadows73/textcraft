long point_create(x, y)
int x;
int y;
{
	return (x << 16) + y;
}

int point_getx(point)
long point;
{
	return point >> 16; 
}

int point_gety(point)
long point;
{
	return point & 65535;
}

int point_equals(point0, point1)
long point0;
long point1;
{
	return point0 == point1;
}

long point_dist2(point0, point1)
long point0;
long point1;
{
	int x0 = point_getx(point0);
	int y0 = point_gety(point0);
	int x1 = point_getx(point1);
	int y1 = point_gety(point1);
	
	int dx = x0 - x1;
	int dy = y0 - y1;

	return dx * dx + dy * dy;
}

int point_adjacent(point0, point1)
long point0;
long point1;
{	
	int x0 = point_getx(point0);
	int y0 = point_gety(point0);
	int x1 = point_getx(point1);
	int y1 = point_gety(point1);
	
	int dx = x0 - x1;
	int dy = y0 - y1;

	if (dx < -1 || dx > 1)
	{
		return 0;
	}
	if (dy < -1 || dy > 1)
	{
		return 0;
	}

	return 1;
}
