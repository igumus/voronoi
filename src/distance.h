#ifndef DISTANCE_H
#define DISTANCE_H

#include <math.h>

typedef int (*distance_fn)(int, int, int, int);

int euclidean(int x1, int y1, int x2, int y2)
{
    int dx = x1 - x2;
    int dy = y1 - y2;
    int ret = sqrt((dx * dx) + (dy * dy));
    return ret;
}

int manhattan(int x1, int y1, int x2, int y2) 
{
    return abs(x1-x2) + abs(y1-y2);
}

int chebyshev(int x1, int y1, int x2, int y2) 
{
    int dx = abs(x1-x2);
    int dy = abs(y1-y2);
    if (dy > dx) return dy; 
    return dx;
}

#endif  // DISTANCE_H
