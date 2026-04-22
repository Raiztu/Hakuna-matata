#include "Game.hpp" 
#include "InputHandler.hpp"
#include "TextureManager.hpp"

#include <iostream>

bool Game::init(std::string title, int w, int h, int flags) {

    window_ = SDL_CreateWindow(title.c_str(), w, h, flags);
    if (window_ == nullptr) {
        std::cerr << "window error" << std::endl;
        return false;
    }
    std::cout << "window created" << std::endl;

    renderer_ = SDL_CreateRenderer(window_, NULL);
    if (renderer_ == nullptr) {
        std::cerr << "renderer error" << std::endl;
        return false;
    }
    std::cout << "renderer created" << std::endl;

    
    if (!TextureManager::Instance().load("assets/menu_bg1.png", "menu_bg", renderer_)) {
        std::cerr << "Failed to load menu background" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/play_button.png", "play_btn", renderer_)) {
        std::cerr << "Failed to load play button" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/exit_button.png", "exit_btn", renderer_)) {
        std::cerr << "Failed to load exit button" << std::endl;
        return false;
    }

    playButton_.load("play_btn", 210, 653, 300, 80);
    exitButton_.load("exit_btn", 210, 765, 300, 80);

    return true;
}

void Game::handleMenuClick(int x, int y) {
    float playLeft = 210, playRight = 510, playTop = 653, playBottom = 733;
    float exitLeft = 210, exitRight = 510, exitTop = 765, exitBottom = 845;

    if (x >= playLeft && x <= playRight && y >= playTop && y <= playBottom) {
        std::cout << "Play button clicked! Switching to game..." << std::endl;
        currentState_ = STATE_GAME;
        return;
    }

    if (x >= exitLeft && x <= exitRight && y >= exitTop && y <= exitBottom) {
        std::cout << "Exit button clicked! Quitting..." << std::endl;
        stopGame();  
        return;
    }
}

void Game::render() {
    // Очищаем экран
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    // Рисуем в зависимости от состояния
    if (currentState_ == STATE_MENU) {
        TextureManager::Instance().draw("menu_bg", 0, 0, 720, 960, renderer_);

        playButton_.draw(renderer_);
        exitButton_.draw(renderer_);
    }
    else if (currentState_ == STATE_GAME) {
        SDL_SetRenderDrawColor(renderer_, 100, 100, 255, 255);
        SDL_RenderClear(renderer_);

        std::cout << "Game state - rendering game..." << std::endl;
    }

    SDL_RenderPresent(renderer_);
}

void Game::update() {
    if (currentState_ == STATE_MENU) {

    }
    else if (currentState_ == STATE_GAME) {
    }
}

void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            stopGame();
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (currentState_ == STATE_MENU) {
                    handleMenuClick(event.button.x, event.button.y);
                }
                else if (currentState_ == STATE_GAME) {
                }
            }
            break;

        case SDL_EVENT_KEY_DOWN:
            if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                stopGame();
            }
            break;
        }
    }
}

void Game::clean() {
    std::cout << "Cleaning up..." << std::endl;

    if (currentState_ == STATE_GAME) {
        go.clean();
        main_char.clean();
    }

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}