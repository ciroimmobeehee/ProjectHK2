#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL.h>
#include <SDL_image.h>

class TextureManager {
public:
    static SDL_Texture* loadTexture(const char* fileName, SDL_Renderer* renderer);
    static void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, float scale);
};

#endif