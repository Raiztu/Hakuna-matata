#include "MenuInputHandler.hpp"

MenuInputHandler::MenuInputHandler() : playClicked(false), exitClicked(false), confirmExitConfirmed(false), confirmExitCancelled(false), isConfirmMode(false), helpConfirmed(false), isHelpMode(false) {}

MenuInputHandler::~MenuInputHandler() {}

bool MenuInputHandler::checkButtonClick(int x, int y) {
    float playLeft = 210, playRight = 535, playTop = 653, playBottom = 733;
    float exitLeft = 210, exitRight = 535, exitTop = 765, exitBottom = 845;
    float helpLeft = 30, helpRight = 142, helpTop = 30, helpBottom = 142;

    if (x >= playLeft && x <= playRight && y >= playTop && y <= playBottom) {
        playClicked = true;
        return true;
    }

    if (x >= exitLeft && x <= exitRight && y >= exitTop && y <= exitBottom) {
        enterConfirmMode();  // переключаем в режим подтверждения
        return true;
    }

    if (x >= helpLeft && x <= helpRight && y >= helpTop && y <= helpBottom) {
        enterHelpMode();  // Включаем режим помощи
        return true;
    }

    return false;
}

bool MenuInputHandler::checkConfirmDialogClick(int x, int y) {
    float confirmLeft = 190, confirmRight = 310;
    float confirmTop = 545, confirmBottom = 625;

    float cancelLeft = 410, cancelRight = 530;
    float cancelTop = 545, cancelBottom = 625;

    if (x >= confirmLeft && x <= confirmRight && y >= confirmTop && y <= confirmBottom) {
        confirmExitConfirmed = true;
        return true;
    }

    if (x >= cancelLeft && x <= cancelRight && y >= cancelTop && y <= cancelBottom) {
        confirmExitCancelled = true;
        return true;
    }

    return false;
}

bool MenuInputHandler::checkHelpDialogClick(int x, int y) {
    float IseeLeft = 260, IseeRight = 460;      
    float IseeTop = 545, IseeBottom = 625;       

    if (x >= IseeLeft && x <= IseeRight && y >= IseeTop && y <= IseeBottom) {
        helpConfirmed = true;
        return true;
    }

    return false;
}

void MenuInputHandler::resetFlags() {
    playClicked = false;
    exitClicked = false;
    confirmExitConfirmed = false;
    confirmExitCancelled = false;
    helpConfirmed = false;
}

// Выход
void MenuInputHandler::enterConfirmMode() {
    isConfirmMode = true;
    resetFlags();  
}

void MenuInputHandler::exitConfirmMode() {
    isConfirmMode = false;
    resetFlags();
}

// Помощь
void MenuInputHandler::enterHelpMode() {
    isHelpMode = true;
    isConfirmMode = false;
    resetFlags();
}

void MenuInputHandler::exitHelpMode() {
    isHelpMode = false;
    resetFlags();
}

void MenuInputHandler::onMouseButtonDown(SDL_Event event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        if (LEFT >= 0 && LEFT < static_cast<int>(mouseStates_.size())) {
            mouseStates_[LEFT] = true;
        }
        if (isConfirmMode) {
            checkConfirmDialogClick(event.button.x, event.button.y);
        }
        else if (isHelpMode) {
            checkHelpDialogClick(event.button.x, event.button.y);
        }
        else {
            checkButtonClick(event.button.x, event.button.y);
        }
    }
}

void MenuInputHandler::onKeyDown(SDL_Event event) {
    keystates_ = SDL_GetKeyboardState(nullptr);

    if (isKeyDown(SDL_SCANCODE_ESCAPE)) {
        if (isConfirmMode) {
            confirmExitCancelled = true;  // Escape отменяет диалог выхода
        }
        else if (isHelpMode) {  // Escape закрывает диалог помощи
            exitHelpMode();
        }
        else {
            exitClicked = true;  // Escape выходит из игры
        }
    }
}