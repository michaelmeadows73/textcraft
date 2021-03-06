#ifndef POINT_H
#define POINT_H

long point_create(int, int);

int point_getx(long);

int point_gety(long);

int point_equals(long, long);

double point_dist(long, long);

long point_dist2(long, long);

long point_adjacent(long, long);

#endif
