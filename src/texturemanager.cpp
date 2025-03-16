#include "Header\texturemanager.hpp"

SDL_Texture* TextureManager::loadTexture(const char* fileName, SDL_Renderer* renderer) {
    return IMG_LoadTexture(renderer, fileName);
}

void TextureManager::renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, float scale) {
    SDL_Rect dest = {x, y, (int)(16 * scale), (int)(16 * scale)};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}