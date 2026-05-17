#include "MenuInputHandler.hpp"

MenuInputHandler::MenuInputHandler() : playClicked(false), exitClicked(false), confirmExitConfirmed(false), confirmExitCancelled(false), isConfirmMode(false), helpConfirmed(false), isHelpMode(false), soundConfirmed(false), isSoundMode(false), isDraggingSlider(false), volumeLevel(0.5f)
{
    sliderWidth = 400;
    sliderX = 35 + (650 - sliderWidth) / 2; // 160 (горизонтальный центр)
    sliderY = 305 + 350 / 2 + 20;                // 480 (вертикальный центр)
    updateSliderHandlePosition();
}

MenuInputHandler::~MenuInputHandler() {}

bool MenuInputHandler::checkButtonClick(int x, int y) {
    float playLeft = 210, playRight = 535, playTop = 653, playBottom = 733;
    float exitLeft = 210, exitRight = 535, exitTop = 765, exitBottom = 845;
    float helpLeft = 30, helpRight = 142, helpTop = 30, helpBottom = 142;
    float soundLeft = 578, soundRight = 690, soundTop = 30, soundBottom = 142;

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

    if (x >= soundLeft && x <= soundRight && y >= soundTop && y <= soundBottom) {
        enterSoundMode();  // Включаем режим звука
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

bool MenuInputHandler::checkSoundDialogClick(int x, int y) {
    float okLeft = 260, okRight = 460;
    float okTop = 545, okBottom = 625;

    if (x >= okLeft && x <= okRight && y >= okTop && y <= okBottom) {
        soundConfirmed = true;
        return true;
    }

    return false;
}

void MenuInputHandler::updateSliderHandlePosition() {
    int handleWidth = 40;  //ширина кружка в пикселях
    sliderHandleX = sliderX + (int)(sliderWidth * volumeLevel) - (handleWidth / 2); // X-координата кружка: начало ползунка + смещение от громкости - половина ширины кружка

    int minHandleX = sliderX - (handleWidth / 2); // минимально возможная X-координата кружка 
    int maxHandleX = sliderX + sliderWidth - (handleWidth / 2);  // максимально возможная X-координата кружка
    if (sliderHandleX < minHandleX) sliderHandleX = minHandleX;  // если вышли за левую границу – прижимаем к левой
    if (sliderHandleX > maxHandleX) sliderHandleX = maxHandleX;  // если вышли за правую границу – прижимаем к правой
}

void MenuInputHandler::resetFlags() {
    playClicked = false;
    exitClicked = false;
    confirmExitConfirmed = false;
    confirmExitCancelled = false;
    helpConfirmed = false;
    soundConfirmed = false;
    isDraggingSlider = false;
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

void MenuInputHandler::enterHelpMode() {
    isHelpMode = true;
    isConfirmMode = false;
    isSoundMode = false;
    resetFlags();
}

void MenuInputHandler::exitHelpMode() {
    isHelpMode = false;
    resetFlags();
}

void MenuInputHandler::enterSoundMode() {
    isSoundMode = true;
    isConfirmMode = false;
    isHelpMode = false;
    resetFlags();
}

void MenuInputHandler::exitSoundMode() {
    isSoundMode = false;
    resetFlags();
}

void MenuInputHandler::onMouseButtonDown(SDL_Event event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        if (LEFT >= 0 && LEFT < static_cast<int>(mouseStates_.size())) {
            mouseStates_[LEFT] = true;
        }

        // код обработки клика мыши в режиме звука
        if (isSoundMode) {
            int handleSize = 40; // размер кружка 
            int handleLeft = sliderHandleX; // левая граница кружка по X
            int handleRight = sliderHandleX + handleSize; // правая граница кружка по X
            int handleTop = sliderY - 15;  // верхняя граница кружка (центр ползунка - 15)
            int handleBottom = sliderY + 15; // нижняя граница кружка (центр ползунка + 15)

            // если клик мыши попал в область кружка
            if (event.button.x >= handleLeft && event.button.x <= handleRight &&
                event.button.y >= handleTop && event.button.y <= handleBottom) {
                isDraggingSlider = true; //// начинаем перетаскивание кружка
            }
            else {
                checkSoundDialogClick(event.button.x, event.button.y); //проверяем клик по кнопке "OK"
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

void MenuInputHandler::onMouseMotion(SDL_Event event) {
    if (isSoundMode && isDraggingSlider) {
        int handleWidth = 40;
        int mouseX = event.motion.x;

        int newHandleX = mouseX - (handleWidth / 2);

        int minHandleX = sliderX - (handleWidth / 2);
        int maxHandleX = sliderX + sliderWidth - (30 / 2);
        if (newHandleX < minHandleX) newHandleX = minHandleX;
        if (newHandleX > maxHandleX) newHandleX = maxHandleX;

        sliderHandleX = newHandleX;

        volumeLevel = (float)(sliderHandleX - minHandleX) / (maxHandleX - minHandleX);
        if (volumeLevel < 0.0f) volumeLevel = 0.0f;
        if (volumeLevel > 1.0f) volumeLevel = 1.0f;
    }
}

void MenuInputHandler::onMouseButtonUp(SDL_Event event) { // //завершение перетаскивания кружка, когда лкм не нажата
    if (event.button.button == SDL_BUTTON_LEFT) {
        isDraggingSlider = false;
        if (LEFT >= 0 && LEFT < static_cast<int>(mouseStates_.size())) { //// проверяем, что индекс левой кнопки (LEFT) в пределах массива состояний мыши
            mouseStates_[LEFT] = false;
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