// #include "Game.hpp"
// #include "SDLHelper.hpp"
// #include <iostream>
// #include <cmath>

// const int SCREEN_WIDTH = 800;
// const int SCREEN_HEIGHT = 600;
// const float FRICTION = 0.98f;
// const float BOUNCE = 0.8f;
// const int BALL_WIDTH = 16;
// const int BALL_HEIGHT = 16;

// Game::Game() 
//     : window(nullptr), renderer(nullptr), ball(nullptr),
//       velocityX(0), velocityY(0), ballX(SCREEN_WIDTH / 2 - BALL_WIDTH / 2),
//       ballY(SCREEN_HEIGHT / 2 - BALL_HEIGHT / 2), dragging(false) {
    
//     window = initSDL("MiniGolf", SCREEN_WIDTH, SCREEN_HEIGHT);
//     renderer = createRenderer(window);
//     ball = loadTexture("ball.png", renderer);
//     if (ball == nullptr) {
//         logErrorAndExit("loadTexture", SDL_GetError());
//     }
    
// }
// Game::~Game() {
//     SDL_DestroyTexture(ball);
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();
//     IMG_Quit();
// }
// void Game::run() {
//     bool quit = false;
//     while (!quit) {
//         handleEvents();
//         update();
//         render();
//         SDL_Delay(16);
//     }
// }
// void Game::handleEvents() {
//     SDL_Event event;
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_QUIT) {
//                 exit(0);
//             }
//             else if(event.type == SDL_MOUSEBUTTONDOWN){
//                 startX = event.button.x;
//                 startY = event.button.y;
//                 dragging = true;
//             }
//             else if(event.type == SDL_MOUSEBUTTONUP && dragging && velocityX == 0 && velocityY == 0){
//                 int endX = event.button.x;
//                 int endY = event.button.y;
//                 velocityX = (startX - endX) * 0.1f;
//                 velocityY = (startY - endY) * 0.1f;
//                 dragging = false;
//             }     
//         }
// }
// void Game::update() {
//     ballX = ballX + velocityX;
//     ballY = ballY + velocityY;

//     wallCollision();

//     velocityX = velocityX * FRICTION;
//     velocityY = velocityY * FRICTION;

//     if(fabs(velocityX) < 0.1f){
//         velocityX = 0;
//     }
//     if(fabs(velocityY) < 0.1f){
//         velocityY = 0;
//     }
// }
// void Game::render() {

// }