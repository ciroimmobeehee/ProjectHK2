#include "Header\renderer.hpp"
#include  <iostream>

bool Renderer::init(SDL_Window*& window, SDL_Renderer*& renderer, const char* title, int width, int height) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "SDL_Init: %s\n", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if(window == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "SDL_CreateWindow: %s\n", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "SDL_CreateRenderer: %s\n", SDL_GetError());
        return false;
    }
    return true;
}
void Renderer::cleanup(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}