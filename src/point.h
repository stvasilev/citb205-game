#ifndef POINT_H
#define POINT_H

#include <list>

using std::list;

struct Point
{
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

#endif