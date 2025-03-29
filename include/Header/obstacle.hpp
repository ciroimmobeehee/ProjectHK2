#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP
#include <SDL.h>

struct Obstacle {
    int x , y;
    int width, height;
    SDL_Rect rect;
    SDL_Texture* texture;

    Obstacle(int _x, int _y, int _width, int _height, SDL_Texture* _tex) {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
        texture = _tex;
        rect = {_x, _y, _width, _height};
    }
};


#endif
