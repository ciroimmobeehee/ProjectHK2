#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP 

struct Obstacle {
    int x , y;
    int width, height;
};

extern Obstacle obstacles[];
extern const int numObstacles;

#endif
