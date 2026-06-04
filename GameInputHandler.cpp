#include "GameInputHandler.hpp"
#include <iostream>

GameInputHandler::GameInputHandler() : lastPressedLetter_(-1), backToMenuClicked(false), isWinDialogMode(false), isLoseDialogMode(false), newGameFromWin(false), backToMenuFromWin(false), newGameFromLose(false), backToMenuFromLose(false)
{
    letters_ = {
        u8"Й", u8"Ц", u8"У", u8"К", u8"Е", u8"Н", u8"Г", u8"Ш", u8"Щ", u8"З", u8"Х", u8"Ъ",
        u8"Ф", u8"Ы", u8"В", u8"А", u8"П", u8"Р", u8"О", u8"Л", u8"Д", u8"Ж", u8"Э",
        u8"Я", u8"Ч", u8"С", u8"М", u8"И", u8"Т", u8"Ь", u8"Б", u8"Ю"
    };

    for (size_t i = 0; i < letters_.size(); i++) {
        letterStates_.push_back(false);
    }

    const float buttonWidth = 54.0f; 
    const float buttonHeight = 58.0f; 
    const float gap = 5.0f; 
    const float screenWidth = 720.0f; 

    float row1_width = 12 * buttonWidth + 11 * gap;
    float row1_startX = (screenWidth - row1_width) / 2;
    float y1 = 700;  

    for (int i = 0; i < 12; i++) {
        addLetterRect(row1_startX + i * (buttonWidth + gap), y1, buttonWidth, buttonHeight);
    }

    float row2_width = 11 * buttonWidth + 10 * gap;
    float row2_startX = (screenWidth - row2_width) / 2;
    float y2 = y1 + buttonHeight + gap;

    for (int i = 0; i < 11; i++) {
        addLetterRect(row2_startX + i * (buttonWidth + gap), y2, buttonWidth, buttonHeight);
    }

    float row3_width = 9 * buttonWidth + 8 * gap;
    float row3_startX = (screenWidth - row3_width) / 2;
    float y3 = y2 + buttonHeight + gap;

    for (int i = 0; i < 9; i++) {
        addLetterRect(row3_startX + i * (buttonWidth + gap), y3, buttonWidth, buttonHeight);
    }
}

void GameInputHandler::addLetterRect(float x, float y, float w, float h) {
    SDL_FRect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    letterRects_.push_back(rect);
}

GameInputHandler::~GameInputHandler() {}

bool GameInputHandler::checkLetterClick(int x, int y, int& letterIndex) { 
    for (size_t i = 0; i < letterRects_.size(); i++) {
        if (x >= letterRects_[i].x && x <= letterRects_[i].x + letterRects_[i].w && y >= letterRects_[i].y && y <= letterRects_[i].y + letterRects_[i].h) {
            letterIndex = i;
            return true;
        }
    }
    return false;
}

void GameInputHandler::getLetterRect(int index, float& x, float& y, float& w, float& h) const {
    if (index >= 0 && index < (int)letterRects_.size()) {
        x = letterRects_[index].x;
        y = letterRects_[index].y;
        w = letterRects_[index].w;
        h = letterRects_[index].h;
    }
}

void GameInputHandler::resetLetters() {
    for (size_t i = 0; i < letterStates_.size(); i++) {
        letterStates_[i] = false;
    }
    lastPressedLetter_ = -1;
    backToMenuClicked = false; 
}

void GameInputHandler::onMouseButtonDown(SDL_Event event) {
    InputHandler::onMouseButtonDown(event);

    if (event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        if (isWinDialogMode) {
            checkWinDialogClick(mouseX, mouseY);
            return; 
        }

        if (isLoseDialogMode) {
            checkLoseDialogClick(mouseX, mouseY);
            return;
        }

        if (checkHelpIconClick(mouseX, mouseY)) {
            return;
        }

        if (checkSoundIconClick(mouseX, mouseY)) {
            return;
        }

        if (isSoundMode) {
            int handleSize = 40;
            int handleLeft = sliderHandleX;
            int handleRight = sliderHandleX + handleSize;
            int handleTop = sliderY - 15;
            int handleBottom = sliderY + 15;

            if (mouseX >= handleLeft && mouseX <= handleRight &&
                mouseY >= handleTop && mouseY <= handleBottom) {
                isDraggingSlider = true;
            }
            else {
                checkSoundDialogClick(mouseX, mouseY);
            }
        }
        else if (isHelpMode) {
            checkHelpDialogClick(mouseX, mouseY);
        }
        else {
            checkBackButtonClick(mouseX, mouseY);

            if (!backToMenuClicked) {
                int letterIndex = -1; 
                if (checkLetterClick(mouseX, mouseY, letterIndex)) {
                    if (!letterStates_[letterIndex]) { 
                        letterStates_[letterIndex] = true; 
                        lastPressedLetter_ = letterIndex; 
                        std::cout << "Letter clicked: " << letters_[letterIndex] << std::endl;
                    }
                }
            }
        }
    }
}

void GameInputHandler::onKeyDown(SDL_Event event) {
    keystates_ = SDL_GetKeyboardState(nullptr);

    if (isKeyDown(SDL_SCANCODE_ESCAPE)) {
        if (isHelpMode) {
            exitHelpMode();
        }
        else if (isSoundMode) {
            exitSoundMode();
        }
        else {
            backToMenuClicked = true;
        }
    }
}

void GameInputHandler::checkBackButtonClick(int x, int y) {
    const float backButtonX = 20; const float backButtonY = 890;
    const float backButtonW = 50; const float backButtonH = 50;

    if (x >= backButtonX && x <= backButtonX + backButtonW &&
        y >= backButtonY && y <= backButtonY + backButtonH) {
        backToMenuClicked = true;
        std::cout << "Back to menu button clicked" << std::endl;
    }
}

void GameInputHandler::enterWinDialog() {
    isWinDialogMode = true;
    isHelpMode = false;
    isSoundMode = false;
    newGameFromWin = false;
    backToMenuFromWin = false;
}

void GameInputHandler::exitWinDialog() {
    isWinDialogMode = false;
    newGameFromWin = false;
    backToMenuFromWin = false;
}

void GameInputHandler::enterLoseDialog() {
    isLoseDialogMode = true;
    isHelpMode = false;
    isSoundMode = false;
    newGameFromLose = false;
    backToMenuFromLose = false;
}

void GameInputHandler::exitLoseDialog() {
    isLoseDialogMode = false;
    newGameFromLose = false;
    backToMenuFromLose = false;
}

void GameInputHandler::resetDialogFlags() {
    newGameFromWin = false;
    backToMenuFromWin = false;
    newGameFromLose = false;
    backToMenuFromLose = false;
};

bool GameInputHandler::checkWinDialogClick(int x, int y) {
    float newGameLeft = 190, newGameRight = 310;
    float newGameTop = 545, newGameBottom = 625;

    float menuLeft = 410, menuRight = 530;
    float menuTop = 545, menuBottom = 625;

    if (x >= newGameLeft && x <= newGameRight && y >= newGameTop && y <= newGameBottom) {
        newGameFromWin = true;
        std::cout << "New game button clicked in win dialog" << std::endl;
        return true;
    }

    if (x >= menuLeft && x <= menuRight && y >= menuTop && y <= menuBottom) {
        backToMenuFromWin = true;
        std::cout << "Menu button clicked in win dialog" << std::endl;
        return true;
    }

    return false;
}

bool GameInputHandler::checkLoseDialogClick(int x, int y) {
    float newGameLeft = 190, newGameRight = 310;
    float newGameTop = 545, newGameBottom = 625;

    float menuLeft = 410, menuRight = 530;
    float menuTop = 545, menuBottom = 625;

    if (x >= newGameLeft && x <= newGameRight && y >= newGameTop && y <= newGameBottom) {
        newGameFromLose = true;
        std::cout << "New game button clicked in lose dialog" << std::endl;
        return true;
    }

    if (x >= menuLeft && x <= menuRight && y >= menuTop && y <= menuBottom) {
        backToMenuFromLose = true;
        std::cout << "Menu button clicked in lose dialog" << std::endl;
        return true;
    }

    return false;
}