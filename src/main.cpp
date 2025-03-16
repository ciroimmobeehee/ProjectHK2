#include <iostream>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "MiniGolf";

const int BALL_WIDTH = 16;
const int BALL_HEIGHT = 16;
const float FRICTION = 0.98f;
const float BOUNCE = 0.9f;

float velocityX = 0;
float velocityY = 0;

// float ballX = SCREEN_WIDTH / 2 - BALL_WIDTH / 2;
// float ballY = SCREEN_HEIGHT / 2 - BALL_HEIGHT / 2;
float ballScale = 1.0f;
float ballX = 50;
float ballY = 300;

float holeX = 700;
float holeY = 300;

bool dragging = false;
int startX , startY;


void logErrorAndExit(const char* msg , const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s\n", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL(const char* WINDOW_TITLE, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logErrorAndExit("SDL_Init", SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logErrorAndExit("SDL_CreateWindow", SDL_GetError());
    }
    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        logErrorAndExit("SDL_CreateRenderer", SDL_GetError());
    }
    return renderer;
}

SDL_Texture* loadTexture(const char* file_name, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, file_name);
    if (texture == nullptr) {
        logErrorAndExit("IMG_LoadTexture", SDL_GetError());
    }
    return texture;
}

void renderTexture(SDL_Texture* texture , SDL_Renderer* renderer , int x , int y , float scale){
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = static_cast<int>(BALL_WIDTH * scale);
    dest.h = static_cast<int>(BALL_HEIGHT * scale);
    //SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void waitUntilKeyPressed() {
    SDL_Event event;
    while (true) {
        if (SDL_WaitEvent(&event) != 0 &&
            (event.type == SDL_KEYDOWN || event.type == SDL_QUIT)) {
                return;
            }
        SDL_Delay(100);
    }
}

void wallCollision() {
    if (ballX < 0) {
        ballX = 0;
        velocityX = -velocityX * BOUNCE;
    }
    else if(ballX + BALL_WIDTH > SCREEN_WIDTH) {
        ballX = SCREEN_WIDTH - BALL_WIDTH;
        velocityX = -velocityX * BOUNCE;
    }
    else if(ballY < 0) {
        ballY = 0;
        velocityY = -velocityY * BOUNCE;
    }
    else if(ballY + BALL_HEIGHT > SCREEN_HEIGHT) {
        ballY = SCREEN_HEIGHT - BALL_HEIGHT;
        velocityY = -velocityY * BOUNCE;
    }
}
void holeCollision() {
    float ballCenterX = ballX + (BALL_WIDTH * ballScale) / 2;
    float ballCenterY = ballY + (BALL_HEIGHT * ballScale) / 2;
    float holeCenterX = holeX + BALL_WIDTH / 2;
    float holeCenterY = holeY + BALL_HEIGHT / 2;
    float holeRadius = BALL_WIDTH / 2; 
    float distance = sqrt(pow(ballCenterX - holeCenterX, 2) + pow(ballCenterY - holeCenterY, 2));
    if (distance < holeRadius && fabs(velocityX) < 20.0f && fabs(velocityY) < 20.0f) {
        if (ballScale > 0.3f) {
            ballScale *= 0.9f; 
            ballX = holeCenterX - (BALL_WIDTH * ballScale) / 2;
            ballY = holeCenterY - (BALL_HEIGHT * ballScale) / 2;
        } else {
    
            ballX = 50;
            ballY = 300;
            velocityX = 0;
            velocityY = 0;
        }
    }
}
void gameLoop(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* ball, SDL_Texture* background, SDL_Texture* hole) {
    bool quit = false;
    while(!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN){
                startX = event.button.x;
                startY = event.button.y;
                dragging = true;
            }
            else if(event.type == SDL_MOUSEBUTTONUP && dragging && velocityX == 0 && velocityY == 0){
                int endX = event.button.x;
                int endY = event.button.y;
                velocityX = (startX - endX) * 0.1f;
                velocityY = (startY - endY) * 0.1f;
                dragging = false;
            }     
        }
        ballX = ballX + velocityX;
        ballY = ballY + velocityY;

        wallCollision();

        velocityX = velocityX * FRICTION;
        velocityY = velocityY * FRICTION;


        if(fabs(velocityX) < 0.1f){
            velocityX = 0;
        }
        if(fabs(velocityY) < 0.1f){
            velocityY = 0;
        }
        
        holeCollision();

        if(ballX == 50  && ballY == 300 && velocityX == 0 && velocityY == 0){
            ballScale = 1.0f;
        }
        //SDL_SetRenderDrawColor(renderer, 0 , 153 , 0 , 255);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        renderTexture(hole, renderer, static_cast<int>(holeX) , static_cast<int>(holeY) , 1.0f);
        renderTexture(ball, renderer, static_cast<int>(ballX) , static_cast<int>(ballY) , ballScale);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
}

int main(int argc, char* argv[]){
    SDL_Window* window = initSDL(WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Renderer* renderer = createRenderer(window);
    
    SDL_Texture* ball = loadTexture("ball.png", renderer);
    SDL_Texture* background = loadTexture("background.jpg", renderer);
    SDL_Texture* hole = loadTexture("hole.png", renderer);

    gameLoop(window, renderer, ball, background, hole);
    
    SDL_DestroyTexture(ball);
    ball = NULL;

    SDL_DestroyTexture(background);
    background = NULL;

    SDL_DestroyTexture(hole);
    hole = NULL;

    quitSDL(window, renderer);
    return 0;
}