#include "MenuInputHandler.hpp"

MenuInputHandler::MenuInputHandler() : playClicked(false), exitClicked(false) {}

MenuInputHandler::~MenuInputHandler() {}

bool MenuInputHandler::checkButtonClick(int x, int y) {
    float playLeft = 210, playRight = 535, playTop = 653, playBottom = 733;
    float exitLeft = 210, exitRight = 535, exitTop = 765, exitBottom = 845;

    if (x >= playLeft && x <= playRight && y >= playTop && y <= playBottom) {
        playClicked = true;
        return true;
    }

    if (x >= exitLeft && x <= exitRight && y >= exitTop && y <= exitBottom) {
        exitClicked = true;
        return true;
    }

    return false;
}

void MenuInputHandler::resetFlags() {
    playClicked = false;
    exitClicked = false;
}

void MenuInputHandler::onMouseButtonDown(SDL_Event event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        if (LEFT >= 0 && LEFT < static_cast<int>(mouseStates_.size())) {
            mouseStates_[LEFT] = true;
        }
        checkButtonClick(event.button.x, event.button.y);
    }
}

void MenuInputHandler::onKeyDown(SDL_Event event) {
    keystates_ = SDL_GetKeyboardState(nullptr);

    if (isKeyDown(SDL_SCANCODE_ESCAPE)) {
        exitClicked = true;
    }
}