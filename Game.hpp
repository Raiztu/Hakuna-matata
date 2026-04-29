#pragma once

#include "GameObject.hpp"
#include "InputHandler.hpp"
#include "MenuInputHandler.hpp"
#include "Player.hpp"

#include <SDL3/SDL.h>
#include <string>

enum GameState {
    STATE_MENU,
    STATE_GAME
};

class Game {
public:
    Game() {}
    ~Game() {}
    bool init(std::string, int, int, int);
    void render();
    void update();
    void handleEvents();
    void clean();
    void startGame() { running_ = true; }
    void stopGame() { running_ = false; }
    bool isRunning() { return running_; }

private:
    bool running_ = false;
    GameState currentState_ = STATE_MENU;
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    // Обработчики ввода
    MenuInputHandler* menuHandler_;
    InputHandler* currentHandler_;


    GameObject playButton_;
    GameObject exitButton_;
    GameObject volumeIcon_;
    GameObject helpIcon_;


    GameObject go;
    Player main_char;
};