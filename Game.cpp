#include "Game.hpp" 
#include "InputHandler.hpp"
#include "MenuInputHandler.hpp"
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

    menuHandler_ = new MenuInputHandler();
    currentHandler_ = menuHandler_;

    // Загрузка текстур
    if (!TextureManager::Instance().load("assets/menu_bg2.png", "menu_bg", renderer_)) {
        std::cerr << "Failed to load menu background" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/play_button1.png", "play_btn", renderer_)) {
        std::cerr << "Failed to load play button" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/exit_button1.png", "exit_btn", renderer_)) {
        std::cerr << "Failed to load exit button" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/volume_da.png", "volume_icon", renderer_)) {
        std::cerr << "Failed to load volume button" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/help1.png", "help_icon", renderer_)) {
        std::cerr << "Failed to load hepl button" << std::endl;
        return false;
    }
    //////////////////////////////////////////// Для выхода
    if (!TextureManager::Instance().load("assets/confirm_bg1.png", "confirm_bg", renderer_)) {
        std::cerr << "Warning: confirm_bg.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/yes_button.png", "confirm_btn", renderer_)) {
        std::cerr << "Warning: confirm_btn.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/no_button.png", "cancel_btn", renderer_)) {
        std::cerr << "Warning: cancel_btn.png not loaded" << std::endl;
    }
    //////////////////////////////////////////// Для помощи
    if (!TextureManager::Instance().load("assets/confirm_bg1.png", "help_bg", renderer_)) {
        std::cerr << "Warning: confirm_bg.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/Isee_button.png", "Isee_btn", renderer_)) {
        std::cerr << "Warning: confirm_btn.png not loaded" << std::endl;
    }

    // Настраиваем
    playButton_.load("play_btn", 198, 653, 325, 80);
    exitButton_.load("exit_btn", 198, 765, 325, 80);
    volumeIcon_.load("volume_icon", 578, 30, 112, 112);
    helpIcon_.load("help_icon", 30, 30, 112, 112);

    /////////////////////////////////// Для выхода
    confirmDialogBg_.load("confirm_bg", 35, 305, 650, 350);
    confirmButton_.load("confirm_btn", 190, 545, 120, 80);
    cancelButton_.load("cancel_btn", 410, 545, 120, 80);

    /////////////////////////////////// Для помощи
    helpDialogBg_.load("help_bg", 35, 305, 650, 350);
    IseeButton_.load("Isee_btn", 300, 545, 120, 80);  
    return true;
}

// Уже непосредственно сама отрисовка
void Game::render() {
    // Очищаем экран
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    // Рисуем в зависимости от состояния
    if (currentState_ == STATE_MENU) {
        TextureManager::Instance().draw("menu_bg", 0, 0, 720, 960, renderer_);

        if (menuHandler_->isConfirmMode) {
            // Диалог выхода
            confirmDialogBg_.draw(renderer_);
            confirmButton_.draw(renderer_);
            cancelButton_.draw(renderer_);

            // для красоты
            volumeIcon_.draw(renderer_);
            helpIcon_.draw(renderer_);
        }
        else if (menuHandler_->isHelpMode) {  // Диалог помощи
            helpDialogBg_.draw(renderer_);
            IseeButton_.draw(renderer_);

            // для красоты
            volumeIcon_.draw(renderer_);
            helpIcon_.draw(renderer_);
        }
        else {
            // Рисуем кнопки меню
            playButton_.draw(renderer_);
            exitButton_.draw(renderer_);
            volumeIcon_.draw(renderer_);
            helpIcon_.draw(renderer_);
        }
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
        default:
            if (menuHandler_) {
                menuHandler_->handle(event);
            }
            break;
        }
    }

    if (currentState_ == STATE_MENU && menuHandler_) {
        if (menuHandler_->isConfirmMode) {
            // В режиме подтверждения
            if (menuHandler_->confirmExitConfirmed) {
                stopGame();  // Выходим
            }
            if (menuHandler_->confirmExitCancelled) {
                menuHandler_->exitConfirmMode();  // Возвращаемся в меню
            }
        }
        else if (menuHandler_->isHelpMode) {  // Режим помощи
            if (menuHandler_->helpConfirmed) {
                menuHandler_->exitHelpMode();  // Закрываем диалог помощи
            }
        }
        else {
            // Обычный режим меню
            if (menuHandler_->playClicked) {
                std::cout << "Play button clicked! Switching to game..." << std::endl;
                currentState_ = STATE_GAME;
                menuHandler_->resetFlags();
            }
            if (menuHandler_->exitClicked) {
                std::cout << "Exit button clicked! Quitting..." << std::endl;
                stopGame();
            }
        }
    }
}

void Game::clean() {
    std::cout << "Cleaning up..." << std::endl;

    delete menuHandler_;
    menuHandler_ = nullptr;
    currentHandler_ = nullptr;

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}