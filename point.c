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
