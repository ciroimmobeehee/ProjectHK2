#include "Header\game.hpp"
#include "Header\renderer.hpp"
#include "Header\texturemanager.hpp"
#include "Header\obstacle.hpp"
#include "Header\level.hpp"
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
    currentLevel = 0;
}
Game::~Game() {}

bool Game::init() {
    if(!Renderer::init(window, renderer, "MiniGolf", SCREEN_WIDTH, SCREEN_HEIGHT)) {
        return false;
    }
    ballTexture = TextureManager::loadTexture("assets/ball.png", renderer);
    if (!ballTexture) {
        std::cerr << "Failed to load ball texture!" << std::endl;
        return false;
    }
    arrowTexture = TextureManager::loadTexture("assets/arrow.png", renderer);
    if (!arrowTexture) {
        std::cerr << "Failed to load arrow texture!" << std::endl;
        return false;
    }
    backgroundTexture = TextureManager::loadTexture("assets/background.png", renderer);
    if (!backgroundTexture) {
        std::cerr << "Failed to load background texture!" << std::endl;
        return false;
    }
    holeTexture = TextureManager::loadTexture("assets/hole.png", renderer);
    if (!holeTexture) {
        std::cerr << "Failed to load hole texture!" << std::endl;
        return false;
    }
    obstacleTexture1 = TextureManager::loadTexture("assets/tile64_dark.png", renderer);
    obstacleTexture2 = TextureManager::loadTexture("assets/tile32_dark.png", renderer);
    obstacleTexture3 = TextureManager::loadTexture("assets/tile32_light.png", renderer);
    if (!obstacleTexture1 || !obstacleTexture2 || !obstacleTexture3) {
        std::cerr << "Failed to load obstacle textures!" << std::endl;
    }
    loadLevels();
    initLevel();
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
    SDL_DestroyTexture(arrowTexture);
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
        else if (event.type == SDL_MOUSEMOTION && dragging){
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;
            arrowAngle = atan2(ballY - mouseY, ballX - mouseX) * 180 / M_PI;
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
    bool ballStopped = (fabs(velocityX) < 0.1f && fabs(velocityY) < 0.1f);
    if(ballStopped) {
        arrowX = ballX + Game::BALL_WIDTH / 2;
        arrowY = ballY + Game::BALL_HEIGHT / 2;
    }else{
        arrowX = -100;
        arrowY = -100;
        arrowAngle = 0;
    }

    holeCollision();

    if(ballX == 50  && ballY == 300 && velocityX == 0 && velocityY == 0) {
        ballScale = 1.0f;
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    if(fabs(velocityX) < 0.1f && fabs(velocityY) < 0.1f) {
        int arrowWidth, arrowHeight;
        SDL_QueryTexture(arrowTexture, NULL, NULL, &arrowWidth, &arrowHeight);
        SDL_Rect dest = {(int)arrowX - arrowWidth / 2, (int)arrowY - arrowHeight / 2, arrowWidth, arrowHeight};
        SDL_RenderCopyEx(renderer, arrowTexture, NULL, &dest, arrowAngle, NULL, SDL_FLIP_NONE);
        if (!arrowTexture) {
            std::cerr << "Arrow texture is null!" << std::endl;
        }
    }
    for (const auto& obstacle : obstacles) {
        if (!obstacle.texture) {
            std::cerr << "Obstacle texture is null!" << std::endl;
            continue;
        }
        SDL_Rect dest = { obstacle.x, obstacle.y, obstacle.width, obstacle.height };
        SDL_RenderCopy(renderer, obstacle.texture, NULL, &dest);
    }
    TextureManager::renderTexture(holeTexture, renderer, static_cast<int>(holeX), static_cast<int>(holeY), 1.0f);
    if (!holeTexture) {
        std::cerr << "Hole texture is null!" << std::endl;
    }
    TextureManager::renderTexture(ballTexture, renderer, static_cast<int>(ballX), static_cast<int>(ballY), ballScale);
    if (!ballTexture) {
        std::cerr << "Ball texture is null!" << std::endl;
    }
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
            currentLevel++;
            int size = levels.size();
            if(currentLevel < size) {
                initLevel();
            } else {
                currentLevel = 0;
                initLevel();
            }
            ballX = 50;
            ballY = 300;
            velocityX = 0;
            velocityY = 0;
            ballScale = 1.0f;
        }
    }
}

void Game::obstacleCollision(){
    for (auto &obs : levels[currentLevel].getObstacles()) {
        if (ballX + BALL_WIDTH > obs.x &&
            ballX < obs.x + obs.width &&
            ballY + BALL_HEIGHT > obs.y &&
            ballY < obs.y + obs.height) {

            float overlapX = (velocityX > 0) ? 
                (ballX + BALL_WIDTH - obs.x) : 
                (obs.x + obs.width - ballX);
            float overlapY = (velocityY > 0) ? 
                (ballY + BALL_HEIGHT - obs.y) : 
                (obs.y + obs.height - ballY);
            
            if (overlapX < overlapY) {
                if (velocityX > 0) {
                    ballX -= overlapX;
                } else {
                    ballX += overlapX;
                }
                velocityX = -velocityX * BOUNCE;
            } else {
                if (velocityY > 0) {
                    ballY -= overlapY;
                } else {
                    ballY += overlapY;
                }
                velocityY = -velocityY * BOUNCE;
            }
        }
    }
}

void Game::loadLevels(){
    levels.clear();
    levels.push_back(Level({}, {700, 300}));
    levels.push_back(Level({{368, 268, 64, 67, obstacleTexture1}, {500, 100, 32, 35, obstacleTexture2}, {500, 468, 32, 35, obstacleTexture3}}, {700, 300}));
    if (!obstacleTexture1 || !obstacleTexture2 || !obstacleTexture3) {
        std::cerr << "Failed to load obstacle textures!" << std::endl;
    }
}

void Game::initLevel(){
    int size = levels.size();
    if(currentLevel >= size){
        return;
    }
    Level& level = levels[currentLevel];

    holeX = level.getHolePosition().x;
    holeY = level.getHolePosition().y;

    const vector<Obstacle>& levelObstacles = level.getObstacles();
    obstacles = levelObstacles;

    numObstacles = obstacles.size();

    ballX = 50;
    ballY = 300;
    velocityX = 0;
    velocityY = 0;
    ballScale = 1.0f;
    
}