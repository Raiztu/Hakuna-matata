#include "GameInputHandler.hpp"
#include <iostream>

GameInputHandler::GameInputHandler() : lastPressedLetter_(-1), backToMenuClicked(false)
{
    letters_ = {
        u8"Й", u8"Ц", u8"У", u8"К", u8"Е", u8"Н", u8"Г", u8"Ш", u8"Щ", u8"З", u8"Х", u8"Ъ",
        u8"Ф", u8"Ы", u8"В", u8"А", u8"П", u8"Р", u8"О", u8"Л", u8"Д", u8"Ж", u8"Э",
        u8"Я", u8"Ч", u8"С", u8"М", u8"И", u8"Т", u8"Ь", u8"Б", u8"Ю"
    };

    // Инициализация состояний
    for (size_t i = 0; i < letters_.size(); i++) {
        letterStates_.push_back(false);
    }

    // Параметры клавиатуры
    const float buttonWidth = 54.0f; // Ширина одной кнопки с буквой 
    const float buttonHeight = 58.0f; // Высота одной кнопки 
    const float gap = 5.0f; // Расстояние между кнопками 
    const float screenWidth = 720.0f; // Ширина самого экрана 

    // Ряд 1: 12 клавиш
    float row1_width = 12 * buttonWidth + 11 * gap; // Общая ширина первого ряда
    float row1_startX = (screenWidth - row1_width) / 2; // Начальная X координата для центрирования ряда 
    float y1 = 700;  // Y координата первого ряда

    // Добавляем прямоугольник кнопки (для каждой)
    for (int i = 0; i < 12; i++) {
        addLetterRect(row1_startX + i * (buttonWidth + gap), y1, buttonWidth, buttonHeight);
    }

    // Ряд 2: 11 клавиш - просто центрируем
    float row2_width = 11 * buttonWidth + 10 * gap;
    float row2_startX = (screenWidth - row2_width) / 2;
    float y2 = y1 + buttonHeight + gap;

    for (int i = 0; i < 11; i++) {
        addLetterRect(row2_startX + i * (buttonWidth + gap), y2, buttonWidth, buttonHeight);
    }

    // Ряд 3: 9 клавиш - просто центрируем
    float row3_width = 9 * buttonWidth + 8 * gap;
    float row3_startX = (screenWidth - row3_width) / 2;
    float y3 = y2 + buttonHeight + gap;

    for (int i = 0; i < 9; i++) {
        addLetterRect(row3_startX + i * (buttonWidth + gap), y3, buttonWidth, buttonHeight);
    }
}

// Метод для добавления прямоугольника
void GameInputHandler::addLetterRect(float x, float y, float w, float h) {
    SDL_FRect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    letterRects_.push_back(rect);
}

GameInputHandler::~GameInputHandler() {}

bool GameInputHandler::checkLetterClick(int x, int y, int& letterIndex) { //Инициализируется в Down как -1
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
    backToMenuClicked = false; // Сбрасываем флаг при новой игре
}

void GameInputHandler::onMouseButtonDown(SDL_Event event) {
    // Вызываем базовую обработку
    InputHandler::onMouseButtonDown(event); // Просто отмечаем, что левая кнопка мышки нажата

    if (event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x; // Координаты клика
        int mouseY = event.button.y;

        // Проверяем иконки
        if (checkHelpIconClick(mouseX, mouseY)) {
            return;
        }

        if (checkSoundIconClick(mouseX, mouseY)) {
            return;
        }

        // Проверяем режимы
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

            // Если нажата не кнопка возврата, проверяем буквы
            if (!backToMenuClicked) {
                int letterIndex = -1; // Создаём переменную для индекса буквы
                if (checkLetterClick(mouseX, mouseY, letterIndex)) {
                    if (!letterStates_[letterIndex]) { // Проверяем, не была ли эта буква уже нажата ранее
                        letterStates_[letterIndex] = true; // Отмечаем букву как нажатую
                        lastPressedLetter_ = letterIndex; // Запоминаем последнюю нажатую букву
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

// Кликабельность возврата
void GameInputHandler::checkBackButtonClick(int x, int y) {
    const float backButtonX = 20; const float backButtonY = 890;
    const float backButtonW = 50; const float backButtonH = 50;

    if (x >= backButtonX && x <= backButtonX + backButtonW &&
        y >= backButtonY && y <= backButtonY + backButtonH) {
        backToMenuClicked = true;
        std::cout << "Back to menu button clicked" << std::endl;
    }
}