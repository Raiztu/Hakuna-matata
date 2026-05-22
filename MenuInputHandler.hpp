#pragma once

#include "InputHandler.hpp"

class MenuInputHandler : public InputHandler {
public:
    MenuInputHandler();
    ~MenuInputHandler();

    virtual void onMouseButtonDown(SDL_Event event) override;
    virtual void onKeyDown(SDL_Event event) override;

    // Метод для проверки клика по кнопкам 
    bool checkButtonClick(int x, int y);
    // Метод для проверки кликов в диалоге выхода
    bool checkConfirmDialogClick(int x, int y);

    // Для главного меню
    bool playClicked;
    bool exitClicked;

    // Флаги для определения, в каком мы режиме (чтобы отрисовывать)
    bool isConfirmMode; // true = показываем диалог, false = главное меню

    // Для диалога выхода подтверждения (закрыть диалог)
    bool confirmExitConfirmed; // согласиться
    bool confirmExitCancelled; // остаться

    void resetFlags();
    void enterConfirmMode(); // Вход в режим подтверждения
    void exitConfirmMode(); // Выход из режима подтверждения
};