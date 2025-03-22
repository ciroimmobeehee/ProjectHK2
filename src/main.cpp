#include "Header/game.hpp"

int main(int argc, char* argv[]) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    Game game;
    if (!game.init()) {
        return -1;
    }
    game.run();
    game.cleanup();
    return 0;
}