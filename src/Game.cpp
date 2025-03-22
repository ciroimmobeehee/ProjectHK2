#include "Header\game.hpp"
#include "Header\renderer.hpp"
#include "Header\texturemanager.hpp"
#include "Header\obstacle.hpp"
#include <iostream>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
Game::Game() {
    ballX = 50;
    ballY = 300;
    velocityX = 0;
    velocityY = 0;
    ballScale = 1.0f;
    holeX = 700;
    holeY = 300;
    dragging = false;
    startX = 0;
    startY = 0;
}
Game::~Game() {}

bool Game::init() {
    if(!Renderer::init(window, renderer, "MiniGolf", SCREEN_WIDTH, SCREEN_HEIGHT)) {
        return false;
    }
    ballTexture = TextureManager::loadTexture("assets/ball.png", renderer);
    backgroundTexture = TextureManager::loadTexture("assets/background.jpg", renderer);
    holeTexture = TextureManager::loadTexture("assets/hole.png", renderer);
    obstacleTexture1 = TextureManager::loadTexture("assets/tile64_dark.png", renderer);
    obstacleTexture2 = TextureManager::loadTexture("assets/tile32_dark.png", renderer);
    obstacleTexture3 = TextureManager::loadTexture("assets/tile32_light.png", renderer);
    return true;
}

void Game::run() {
    bool quit = false;
    while(!quit) {
        processEvents();
        update();
        render();
        SDL_Delay(16);
    }
}

void Game::cleanup() {
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(holeTexture);
    SDL_DestroyTexture(obstacleTexture1);
    SDL_DestroyTexture(obstacleTexture2);
    SDL_DestroyTexture(obstacleTexture3);
    Renderer::cleanup(window, renderer);
}

void Game::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            startX = event.button.x;
            startY = event.button.y;
            dragging = true;
        }
        else if (event.type == SDL_MOUSEBUTTONUP && dragging && velocityX == 0 && velocityY == 0) {
            int endX = event.button.x;
            int endY = event.button.y;
            velocityX = (startX - endX) * 0.1f;
            velocityY = (startY - endY) * 0.1f;
            dragging = false;
        }
    }
}

void Game::update() {
    ballX = ballX + velocityX;
    ballY = ballY + velocityY;

    wallCollision();
    obstacleCollision();

    velocityX = velocityX * FRICTION;
    velocityY = velocityY * FRICTION;

    if(fabs(velocityX) < 0.1f) {
        velocityX = 0;
    }
    if(fabs(velocityY) < 0.1f) {
        velocityY = 0;
    }

    holeCollision();

    if(ballX == 50  && ballY == 300 && velocityX == 0 && velocityY == 0) {
        ballScale = 1.0f;
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    for(int i = 0 ; i < numObstacles ; i++) {
        SDL_Texture* obstacleTexture = nullptr;
        if( i == 0 ){
            obstacleTexture = obstacleTexture1;
        }else if( i == 1 ){
            obstacleTexture = obstacleTexture2;
        }else if( i == 2 ){
            obstacleTexture = obstacleTexture3;
        }
        SDL_Rect dest = {obstacles[i].x, obstacles[i].y, obstacles[i].width, obstacles[i].height};
        SDL_RenderCopy(renderer, obstacleTexture, NULL, &dest);
    }
    TextureManager::renderTexture(holeTexture, renderer, static_cast<int>(holeX), static_cast<int>(holeY), 1.0f);
    TextureManager::renderTexture(ballTexture, renderer, static_cast<int>(ballX), static_cast<int>(ballY), ballScale);
    SDL_RenderPresent(renderer);
}

void Game::wallCollision() {
    
    if (ballX < 0) {
        ballX = 0;
        velocityX = -velocityX * BOUNCE;
    }

    if (ballX + BALL_WIDTH > SCREEN_WIDTH) {
        ballX = SCREEN_WIDTH - BALL_WIDTH;
        velocityX = -velocityX * BOUNCE;
    }
    
    if (ballY < 0) {
        ballY = 0;
        velocityY = -velocityY * BOUNCE;
    }
    
    if (ballY + BALL_HEIGHT > SCREEN_HEIGHT) {
        ballY = SCREEN_HEIGHT - BALL_HEIGHT;
        velocityY = -velocityY * BOUNCE;
    }
}

void Game::holeCollision() {
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
            ballScale = 1.0f;
        }
    }
}

void Game::obstacleCollision(){
    for(int i = 0; i < numObstacles; i++) {
        if(ballX + BALL_WIDTH > obstacles[i].x && ballX < obstacles[i].x + obstacles[i].width &&
           ballY + BALL_HEIGHT > obstacles[i].y && ballY < obstacles[i].y + obstacles[i].height) {
            float overlapX =  (velocityX > 0) ? (ballX + BALL_WIDTH - obstacles[i].x)
                                                :(obstacles[i].x + obstacles[i].width - ballX);
            float overlapY = (velocityY > 0) ? (ballY + BALL_HEIGHT - obstacles[i].y)
                                                :(obstacles[i].y + obstacles[i].height - ballY);
            if(overlapX < overlapY){
                if(velocityX > 0){
                    ballX = ballX - overlapX;
                }else{
                    ballX = ballX + overlapX;
                }
                velocityX = -velocityX * BOUNCE;
            }else{
                if(velocityY > 0){
                    ballY = ballY - overlapY;
                }else{
                    ballY = ballY + overlapY;
                }
                velocityY = -velocityY * BOUNCE;
            }
        }
    }
}
