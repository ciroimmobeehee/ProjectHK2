#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Header/obstacle.hpp"
#include <SDL.h>
#include <vector>
using namespace std;

class Level {
public:
    Level(vector<Obstacle> obstacles, SDL_Point holePosition);
    const vector<Obstacle>& getObstacles() const;
    SDL_Point getHolePosition() const;
private:
    vector<Obstacle> obstacles;
    SDL_Point holePosition;
};

#endif 
