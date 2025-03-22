#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP
#include <SDL.h>

struct Obstacle {
    int x , y;
    int width, height;

};

extern Obstacle obstacles[];
extern const int numObstacles;

#endif
