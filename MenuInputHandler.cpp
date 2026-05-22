#include "MenuInputHandler.hpp"

MenuInputHandler::MenuInputHandler() : playClicked(false), exitClicked(false), confirmExitConfirmed(false), confirmExitCancelled(false), isConfirmMode(false) {}

MenuInputHandler::~MenuInputHandler() {}

bool MenuInputHandler::checkButtonClick(int x, int y) {
    float playLeft = 210, playRight = 535, playTop = 653, playBottom = 733;
    float exitLeft = 210, exitRight = 535, exitTop = 765, exitBottom = 845;

    if (x >= playLeft && x <= playRight && y >= playTop && y <= playBottom) {
        playClicked = true;
        return true;
    }

    if (x >= exitLeft && x <= exitRight && y >= exitTop && y <= exitBottom) {
        enterConfirmMode(); // переключаем в режим подтверждения
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

void MenuInputHandler::resetFlags() {
    playClicked = false;
    exitClicked = false;
    confirmExitConfirmed = false;
    confirmExitCancelled = false;
    resetCommonFlags();
}

void MenuInputHandler::enterConfirmMode() {
    isConfirmMode = true;
    isHelpMode = false;
    isSoundMode = false;
    resetFlags();
}

void MenuInputHandler::exitConfirmMode() {
    isConfirmMode = false;
    resetFlags();
}

void MenuInputHandler::onMouseButtonDown(SDL_Event event) {
    // Сначала вызываем базовую обработку (для обновления состояний кнопок)
    InputHandler::onMouseButtonDown(event);

    if (event.button.button == SDL_BUTTON_LEFT) {
        // Проверяем иконки 
        if (checkHelpIconClick(event.button.x, event.button.y)) {
            return;
        }

        if (checkSoundIconClick(event.button.x, event.button.y)) {
            return;
        }

        // Проверяем режимы
        // код обработки клика мыши в режиме звука
        if (isSoundMode) {
            // Проверяем клик по кружку ползунка
            int handleSize = 40; // размер кружка 
            int handleLeft = sliderHandleX; // левая граница кружка по X
            int handleRight = sliderHandleX + handleSize; // правая граница кружка по X
            int handleTop = sliderY - 15; // верхняя граница кружка (центр ползунка - 15)
            int handleBottom = sliderY + 15; // нижняя граница кружка (центр ползунка + 15)

            // если клик мыши попал в область кружка
            if (event.button.x >= handleLeft && event.button.x <= handleRight &&
                event.button.y >= handleTop && event.button.y <= handleBottom) {
                isDraggingSlider = true; // начинаем перетаскивание кружка
            }
            else {
                checkSoundDialogClick(event.button.x, event.button.y); //проверяем клик по кнопке в диалоге тогда
            }
        }
        else if (isConfirmMode) {
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
            confirmExitCancelled = true;
        }
        else if (isHelpMode) {
            exitHelpMode();
        }
        else if (isSoundMode) {
            exitSoundMode();
        }
        else {
            exitClicked = true;
        }
    }
}