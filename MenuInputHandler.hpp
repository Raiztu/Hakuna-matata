#pragma once

#include "InputHandler.hpp"

class MenuInputHandler : public InputHandler {
public:
    MenuInputHandler();
    ~MenuInputHandler();

    virtual void onMouseButtonDown(SDL_Event event) override;
    virtual void onKeyDown(SDL_Event event) override;

    // Метод для проверки клика по кнопкам (будет вызываться из Game)
    bool checkButtonClick(int x, int y);

    // Флаги, которые будет читать Game
    bool playClicked;
    bool exitClicked;

    void resetFlags();
};