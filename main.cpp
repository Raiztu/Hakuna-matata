#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Game.hpp"
#include <clocale>
/// @brief Глобальный указатель на объект игры.
Game* g = 0;

/// @brief Точка входа в программу
/// @param argv Количество аргументов командной строки 
/// @param argc Массив аргументов командной строки 
/// @return 0 при успешном завершении, 1 при ошибке инициализации.
int main(int argv, char** argc) {
    setlocale(LC_ALL, "ru_RU.utf8");
    g = new Game();
    if (g->init("Test Game", 720, 960, SDL_WINDOW_RESIZABLE)) {
        g->startGame();
    }
    else {
        return 1;
    }

    while (g->isRunning()) {
        g->handleEvents();
        g->update();
        g->render();
    }
    g->clean();
    return 0;
}