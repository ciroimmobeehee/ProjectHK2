#include "Header\game.hpp"
#include "Header\renderer.hpp"
#include "Header\texturemanager.hpp"
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
    TextureManager::renderTexture(holeTexture, renderer, static_cast<int>(holeX), static_cast<int>(holeY), 1.0f);
    TextureManager::renderTexture(ballTexture, renderer, static_cast<int>(ballX), static_cast<int>(ballY), ballScale);
    SDL_RenderPresent(renderer);
}

void Game::wallCollision() {
    if (ballX < 0 || ballX + BALL_WIDTH > SCREEN_WIDTH) {
        velocityX = -velocityX * BOUNCE;
    }
    if (ballY < 0 || ballY + BALL_HEIGHT > SCREEN_HEIGHT) {
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
