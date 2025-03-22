#include "Header\texturemanager.hpp"
#include "Header\game.hpp"
#include <cmath>
SDL_Texture* TextureManager::loadTexture(const char* fileName, SDL_Renderer* renderer) {
    return IMG_LoadTexture(renderer, fileName);
}

void TextureManager::renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, float scale) {
    SDL_Rect dest = {x, y, (int)(Game::BALL_WIDTH * scale), (int)(Game::BALL_HEIGHT * scale)};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}