#ifndef GAME_HPP
#define GAME_HPP
#include <SDL.h>
#include <SDL_image.h>
class Game{
    public:
        static inline constexpr int SCREEN_WIDTH = 800;
        static inline constexpr int SCREEN_HEIGHT = 600;
        static inline constexpr int BALL_WIDTH = 16;
        static inline constexpr int BALL_HEIGHT = 16;
        static inline constexpr float FRICTION = 0.98f;
        static inline constexpr float BOUNCE = 0.9f;
        Game();
        ~Game();
        bool init();
        void run();
        void processEvents();
        void update();
        void render();
        void cleanup();
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* ballTexture = nullptr;
        SDL_Texture* backgroundTexture = nullptr;
        SDL_Texture* holeTexture = nullptr;
        SDL_Texture* obstacleTexture1 = nullptr;
        SDL_Texture* obstacleTexture2 = nullptr;
        SDL_Texture* obstacleTexture3 = nullptr;


        float ballX, ballY, velocityX, velocityY, ballScale;
        float holeX, holeY;
        bool dragging;
        int startX, startY;

        void wallCollision();
        void holeCollision();
        void obstacleCollision();

};
#endif