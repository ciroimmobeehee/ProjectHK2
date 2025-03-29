#include "Header/level.hpp"
#include "Header/obstacle.hpp"
#include <SDL.h>
using namespace std;

Level::Level(vector<Obstacle> _obstacles, SDL_Point _holePosition){
    obstacles = _obstacles;
    holePosition = _holePosition;
}
const vector<Obstacle>& Level::getObstacles() const {
    return obstacles;
}

SDL_Point Level::getHolePosition() const {
    return holePosition;
}

