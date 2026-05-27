#include "InputHandler.hpp"

InputHandler::InputHandler() : helpConfirmed(false), soundConfirmed(false), isHelpMode(false), isSoundMode(false), isDraggingSlider(false), volumeLevel(0.5f)
{
    for (int i = 0; i < 5; i++) {
        mouseStates_.push_back(false);
    }
    mousePosition_ = new Vector2D(0, 0);

    // Инициализация ползунка
    sliderWidth = 400;
    sliderX = 35 + (650 - sliderWidth) / 2; // 160 (горизонтальный центр)
    sliderY = 305 + 350 / 2 + 20; // 480 (вертикальный центр)
    updateSliderHandlePosition();
}

Vector2D* InputHandler::getMousePosition() {
    return mousePosition_;
}

bool InputHandler::getMouseButtonState(int buttonNumber) {
    return mouseStates_[buttonNumber];
}

bool InputHandler::isKeyDown(SDL_Scancode key) {
    if (keystates_ != 0) {
        return keystates_[key];
    }
    return false;
}

void InputHandler::handle(SDL_Event event) {
    keystates_ = SDL_GetKeyboardState(nullptr);

    switch (event.type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        onMouseButtonDown(event);
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        onMouseButtonUp(event);
        break;
    case SDL_EVENT_MOUSE_MOTION:
        onMouseMotion(event);
        break;
    case SDL_EVENT_KEY_DOWN:
        onKeyDown(event);
        break;
    default:
        break;
    }
}

// Базовая реализация

void InputHandler::onMouseButtonDown(SDL_Event event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        if (LEFT >= 0 && LEFT < static_cast<int>(mouseStates_.size())) {
            mouseStates_[LEFT] = true;
        }
    }
}

void InputHandler::onMouseButtonUp(SDL_Event event) { //завершение перетаскивания кружка, когда лкм не нажата
    if (event.button.button == SDL_BUTTON_LEFT) {
        isDraggingSlider = false;
        if (LEFT >= 0 && LEFT < static_cast<int>(mouseStates_.size())) { // проверяем, что индекс левой кнопки (LEFT) в пределах массива состояний мыши
            mouseStates_[LEFT] = false;
        }
    }
}

void InputHandler::onMouseMotion(SDL_Event event) {
    if (isSoundMode && isDraggingSlider) {
        int handleWidth = 40;
        int mouseX = event.motion.x;

        int newHandleX = mouseX - (handleWidth / 2);
        int minHandleX = sliderX - (handleWidth / 2);
        int maxHandleX = sliderX + sliderWidth - (handleWidth / 2);

        if (newHandleX < minHandleX) newHandleX = minHandleX;
        if (newHandleX > maxHandleX) newHandleX = maxHandleX;

        sliderHandleX = newHandleX;
        volumeLevel = (float)(sliderHandleX - minHandleX) / (maxHandleX - minHandleX);

        if (volumeLevel < 0.0f) volumeLevel = 0.0f;
        if (volumeLevel > 1.0f) volumeLevel = 1.0f;
    }
}

void InputHandler::onKeyDown(SDL_Event event) {
    keystates_ = SDL_GetKeyboardState(nullptr);

    if (isKeyDown(SDL_SCANCODE_ESCAPE)) {
        if (isHelpMode) {
            exitHelpMode();
        }
        else if (isSoundMode) {
            exitSoundMode();
        }
    }
}

// Методы для иконок звука и помощи
bool InputHandler::checkHelpIconClick(int x, int y) {
    float helpLeft = 30, helpRight = 142, helpTop = 30, helpBottom = 142;
    if (x >= helpLeft && x <= helpRight && y >= helpTop && y <= helpBottom) {
        enterHelpMode();
        return true;
    }
    return false;
}

bool InputHandler::checkSoundIconClick(int x, int y) {
    float soundLeft = 578, soundRight = 690, soundTop = 30, soundBottom = 142;
    if (x >= soundLeft && x <= soundRight && y >= soundTop && y <= soundBottom) {
        enterSoundMode();
        return true;
    }
    return false;
}

bool InputHandler::checkHelpDialogClick(int x, int y) {
    float IseeLeft = 300, IseeRight = 430;
    float IseeTop = 545, IseeBottom = 625;

    if (x >= IseeLeft && x <= IseeRight && y >= IseeTop && y <= IseeBottom) {
        helpConfirmed = true;
        return true;
    }
    return false;
}

bool InputHandler::checkSoundDialogClick(int x, int y) {
    float okLeft = 260, okRight = 460;
    float okTop = 545, okBottom = 625;

    if (x >= okLeft && x <= okRight && y >= okTop && y <= okBottom) {
        soundConfirmed = true;
        return true;
    }
    return false;
}

void InputHandler::updateSliderHandlePosition() {
    int handleWidth = 40; //ширина кружка в пикселях
    sliderHandleX = sliderX + (int)(sliderWidth * volumeLevel) - (handleWidth / 2); // X-координата кружка: начало ползунка + смещение от громкости - половина ширины кружка

    int minHandleX = sliderX - (handleWidth / 2); // минимально возможная X-координата кружка 
    int maxHandleX = sliderX + sliderWidth - (handleWidth / 2); // максимально возможная X-координата кружка
    if (sliderHandleX < minHandleX) sliderHandleX = minHandleX; // если вышли за левую границу – прижимаем к левой
    if (sliderHandleX > maxHandleX) sliderHandleX = maxHandleX; // если вышли за правую границу – прижимаем к правой
}

void InputHandler::resetCommonFlags() {
    helpConfirmed = false;
    soundConfirmed = false;
    isDraggingSlider = false;
}

void InputHandler::enterHelpMode() {
    isHelpMode = true;
    isSoundMode = false;
    resetCommonFlags();
}

void InputHandler::exitHelpMode() {
    isHelpMode = false;
    resetCommonFlags();
}

void InputHandler::enterSoundMode() {
    isSoundMode = true;
    isHelpMode = false;
    resetCommonFlags();
}

void InputHandler::exitSoundMode() {
    isSoundMode = false;
    resetCommonFlags();
}

// Для синхронизации звука между состояниями
float InputHandler::getVolumeLevel() const { return volumeLevel; }
void InputHandler::setVolumeLevel(float level) {
    volumeLevel = level;
    if (volumeLevel < 0.0f) volumeLevel = 0.0f;
    if (volumeLevel > 1.0f) volumeLevel = 1.0f;
    updateSliderHandlePosition();
}