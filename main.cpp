#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Game.hpp"
#include <clocale>
Game* g = 0;

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